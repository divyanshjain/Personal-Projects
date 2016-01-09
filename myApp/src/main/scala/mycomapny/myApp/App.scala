package mycomapny.myApp

import org.apache.spark.SparkConf
import org.apache.spark.SparkContext

import scala.collection.Map
import scala.collection.mutable.ArrayBuffer
import scala.util.Random

import org.apache.spark.{SparkConf, SparkContext}
import org.apache.spark.SparkContext._
import org.apache.spark.broadcast.Broadcast
import org.apache.spark.rdd.RDD
import org.apache.spark.mllib.recommendation._

/**
 * @author ${user.name}
 */
object App {
  
  def foo(x : Array[String]) = x.foldLeft("")((a,b) => a + b)
  
  def main(args : Array[String]) {
    
    println("concat arguments = " + foo(args))
    val conf = new SparkConf().setMaster("local[2]").setAppName("App");
    val sc = new SparkContext(conf);
   
    val userArtistInfo = sc.textFile("C:/Dj/My Stuff/spark/spark-book/aas-master/dataset/profiledata_06-May-2005/user_artist_data.txt");
    val artistInfo = sc.textFile("C:/Dj/My Stuff/spark/spark-book/aas-master/dataset/profiledata_06-May-2005/artist_data.txt");
    val artistDuplicate = sc.textFile("C:/Dj/My Stuff/spark/spark-book/aas-master/dataset/profiledata_06-May-2005/artist_alias.txt");
    //val res = txtfile.filter { x => x.contains("Spark Change Log") }.count();
    //println(res);
    //val s = txtfile.map(_.split(' ')(0).toDouble).stats();
    preprocessData(userArtistInfo, artistInfo, artistDuplicate)
    model(sc, userArtistInfo, artistInfo, artistDuplicate)
    evaluateRecommender(sc, userArtistInfo, artistDuplicate)
    recommend(sc, userArtistInfo, artistInfo, artistDuplicate)
  }

  def preprocessArtistData(artistInfo: RDD[String]) =
    artistInfo.flatMap { line =>
      val (id, name) = line.span(_ != '\t')
      if (name.isEmpty) {
        None
      } else {
        try {
          Some((id.toInt, name.trim))
        } catch {
          case e: NumberFormatException => None
        }
      }
    }
  
  def preprocessArtistDuplicate(artistDuplicate: RDD[String]): Map[Int,Int] =
    artistDuplicate.flatMap { line =>
      val tokens = line.split('\t')
      if (tokens(0).isEmpty) {
        None
      } else {
        Some((tokens(0).toInt, tokens(1).toInt))
      }
    }.collectAsMap()

    def preprocessData(
      userArtistInfo: RDD[String],
      artistInfo: RDD[String],
      artistDuplicate: RDD[String]) = {
      //compute stats for users and items
    val userStats = userArtistInfo.map(_.split(' ')(0).toDouble).stats()
    val itemStats = userArtistInfo.map(_.split(' ')(1).toDouble).stats()
    println(userStats)
    println(itemStats)

    val artistByID = preprocessArtistData(artistInfo)
    val artistAlias = preprocessArtistDuplicate(artistDuplicate)

    val (badID, goodID) = artistAlias.head
    println(artistByID.lookup(badID) + " -> " + artistByID.lookup(goodID))
  }
    
   def buildRatings(
      rawUserArtistData: RDD[String],
      bArtistAlias: Broadcast[Map[Int,Int]]) = {
    rawUserArtistData.map { line =>
      val Array(userID, artistID, count) = line.split(' ').map(_.toInt)
      val finalArtistID = bArtistAlias.value.getOrElse(artistID, artistID)
      Rating(userID, finalArtistID, count)
    }
  }
   
   def model(
      sc: SparkContext,
      userArtistInfo: RDD[String],
      artistInfo: RDD[String],
      artistDuplicate: RDD[String]): Unit = {

    val bArtistAlias = sc.broadcast(preprocessArtistDuplicate(artistDuplicate))

    val trainData = buildRatings(userArtistInfo, bArtistAlias).cache()

    val model = ALS.trainImplicit(trainData, 10, 5, 0.01, 1.0)

    trainData.unpersist()

    println(model.userFeatures.mapValues(_.mkString(", ")).first())

    val userID = 2093760
    val recommendations = model.recommendProducts(userID, 5)
    recommendations.foreach(println)
    val recommendedProductIDs = recommendations.map(_.product).toSet

    val rawArtistsForUser = userArtistInfo.map(_.split(' ')).
      filter { case Array(user,_,_) => user.toInt == userID }

    val existingProducts = rawArtistsForUser.map { case Array(_,artist,_) => artist.toInt }.
      collect().toSet

    val artistByID = preprocessArtistData(artistInfo)

    artistByID.filter { case (id, name) => existingProducts.contains(id) }.
      values.collect().foreach(println)
    artistByID.filter { case (id, name) => recommendedProductIDs.contains(id) }.
      values.collect().foreach(println)

    //manually defined it as to unpersist the matrix factorization model
    unpersist(model)
  }
   
   def areaUnderCurve(
      positiveData: RDD[Rating],
      bAllItemIDs: Broadcast[Array[Int]],
      predictFunction: (RDD[(Int,Int)] => RDD[Rating])) = {
    // AUC is computed per user. 

    // Take withheld data as the +ve and map to corresponding tuples.
    val positiveUserProducts = positiveData.map(r => (r.user, r.product))
    val positivePredictions = predictFunction(positiveUserProducts).groupBy(_.user)

    // -ve items are all items - +ve items
    val negativeUserProducts = positiveUserProducts.groupByKey().mapPartitions {
      
      userIDAndPosItemIDs => {
        
        val random = new Random()
        val allItemIDs = bAllItemIDs.value
        userIDAndPosItemIDs.map { case (userID, posItemIDs) =>
          val posItemIDSet = posItemIDs.toSet
          val negative = new ArrayBuffer[Int]()
          var i = 0
          // no of negative examples per user is approx equal to no of positive examples
          // Duplicates are OKAY.
          while (i < allItemIDs.size && negative.size < posItemIDSet.size) {
            val itemID = allItemIDs(random.nextInt(allItemIDs.size))
            if (!posItemIDSet.contains(itemID)) {
              negative += itemID
            }
            i += 1
          }
          
          negative.map(itemID => (userID, itemID))
        }
      }
    }.flatMap(t => t)
    

    // Make predictions on the rest:
    val negativePredictions = predictFunction(negativeUserProducts).groupBy(_.user)

    // Join positive and negative by user
    positivePredictions.join(negativePredictions).values.map {
      case (positiveRatings, negativeRatings) =>
        
        var correct = 0L
        var total = 0L
        
        for (positive <- positiveRatings;
             negative <- negativeRatings) {
          
          if (positive.rating > negative.rating) {
            correct += 1
          }
          total += 1
        }
        
        correct.toDouble / total
    }.mean() // Return mean AUC over users
  }
   
   def predictMostListened(sc: SparkContext, train: RDD[Rating])(allData: RDD[(Int,Int)]) = {
    val bListenCount =
      sc.broadcast(train.map(r => (r.product, r.rating)).reduceByKey(_ + _).collectAsMap())
    allData.map { case (user, product) =>
      Rating(user, product, bListenCount.value.getOrElse(product, 0.0))
    }
  }
   
   def evaluateRecommender(
      sc: SparkContext,
      rawUserArtistData: RDD[String],
      rawArtistAlias: RDD[String]): Unit = {
    val bArtistAlias = sc.broadcast(preprocessArtistDuplicate(rawArtistAlias))

    val allData = buildRatings(rawUserArtistData, bArtistAlias)
    val Array(trainData, cvData) = allData.randomSplit(Array(0.9, 0.1))
    trainData.cache()
    cvData.cache()

    val allItemIDs = allData.map(_.product).distinct().collect()
    val bAllItemIDs = sc.broadcast(allItemIDs)

    val mostListenedAUC = areaUnderCurve(cvData, bAllItemIDs, predictMostListened(sc, trainData))
    println(mostListenedAUC)

    val evaluations =
      for (rank   <- Array(10,  50);
           lambda <- Array(1.0, 0.0001);
           alpha  <- Array(1.0, 40.0))
      yield {
        // while running on the local machine in a stanalone mode, i observed that the optimal values were rank = 50, iterations = 10, lambda = 0.01 and alpha = 13- 15
        // however, such hyperparameters made the processing computationally expensive. 
        val model = ALS.trainImplicit(trainData, rank, 10, lambda, alpha)
        val auc = areaUnderCurve(cvData, bAllItemIDs, model.predict)
        unpersist(model)
        ((rank, lambda, alpha), auc)
      }

    evaluations.sortBy(_._2).reverse.foreach(println)

    trainData.unpersist()
    cvData.unpersist()
  }
   
   def recommend(
      sc: SparkContext,
      rawUserArtistData: RDD[String],
      rawArtistData: RDD[String],
      rawArtistAlias: RDD[String]): Unit = {

    val bArtistAlias = sc.broadcast(preprocessArtistDuplicate(rawArtistAlias))
    val allData = buildRatings(rawUserArtistData, bArtistAlias).cache()
    val model = ALS.trainImplicit(allData, 50, 10, 1.0, 40.0)
    allData.unpersist()

    val userID = 2093760
    val recommendations = model.recommendProducts(userID, 5)
    val recommendedProductIDs = recommendations.map(_.product).toSet

    val artistByID = preprocessArtistData(rawArtistData)

    artistByID.filter { case (id, name) => recommendedProductIDs.contains(id) }.
       values.collect().foreach(println)

    val someUsers = allData.map(_.user).distinct().take(5)
    val someRecommendations = someUsers.map(userID => model.recommendProducts(userID, 5))
    someRecommendations.map(
      recs => recs.head.user + " -> " + recs.map(_.product).mkString(", ")
    ).foreach(println)

    unpersist(model)
  }
   
   def unpersist(model: MatrixFactorizationModel): Unit = {
    // At the moment, it's necessary to manually unpersist the RDDs inside the model
    // when done with it in order to make sure they are promptly uncached
    model.userFeatures.unpersist()
    model.productFeatures.unpersist()
  }
   
}
