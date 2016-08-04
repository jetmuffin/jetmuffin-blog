title: Hadoop java api ，HDFS文件操作，便于Web开发的集合
tags: [Hadoop,HDFS,云计算]
categories: 云计算
date: 2014-11-06 04:58:45
---

<span style="white-space:pre"></span>最近在搞一个云计算比赛，要开发HDFS存储云，根据一个博客给的API写了一个完整的JAVA文件集成所有的HDFS文件操作，后期用JSP开发时只要导入这个包就OK了。

<!--more-->

<span style="white-space:pre"></span>感谢原作者：[http://www.cnblogs.com/xuqiang/archive/2011/06/03/2042526.html](http://www.cnblogs.com/xuqiang/archive/2011/06/03/2042526.html)

<span style="white-space:pre"></span>代码如下：

<span style="white-space:pre"></span>`import java.io.BufferedInputStream;

import java.io.File;

import java.io.FileInputStream;

import java.io.InputStream;

import java.net.URI;

import org.apache.hadoop.conf.Configuration;

import org.apache.hadoop.fs.BlockLocation;

import org.apache.hadoop.fs.FSDataInputStream;

import org.apache.hadoop.fs.FSDataOutputStream;

import org.apache.hadoop.fs.FileStatus;

import org.apache.hadoop.fs.FileSystem;

import org.apache.hadoop.fs.Path;

import org.apache.hadoop.io.IOUtils;

public class HadoopFileUtil {

/**

 * 	HDFS文件访问API

 * 	@author Jet-Muffin

 */

	  public static String hdfsUrl = "hdfs://localhost:9000";

	  /**

	   * create HDFS folder 创建一个文件夹

	   * @param dirPath

	   * @return

	   */

	    public static void createDir(String dirPath) throws Exception{  

	       Configuration conf= new Configuration();

	       FileSystem hdfs = FileSystem.get(URI.create(hdfsUrl),conf);

	       Path path = new Path(dirPath);

	       hdfs.mkdirs(path);

	       hdfs.close();

	    }

	    /**

	     * delete HDFS folder 删除一个文件夹

	     * @param dirPath

	     * @return

	     */

	    public static void deleteDir(String dirPath) throws Exception{

	        Configuration conf = new Configuration();

	        FileSystem hdfs = FileSystem.get(URI.create(hdfsUrl),conf); 

	        hdfs.delete(new Path(dirPath));        

	        hdfs.close();

	    }

	    /**

	     * create a file  创建一个文件

	     * @param filePath

	     * @return

	     */

	    public static void createFile(String filePath,String content) throws Exception{  

		      Configuration conf= new Configuration();

		      FileSystem hdfs = FileSystem.get(URI.create(hdfsUrl),conf);

		      Path path = new Path(filePath);

		      FSDataOutputStream out = hdfs.create(path);

		      out.write(content.getBytes());

		      out.close();

		      hdfs.close();

	    }

	    /**

	     * rename a file 重命名一个文件

	     * @param oldPath

	     * @param newPath

	     * @return

	     */

	     public static void renameFile(String oldPath,String newPath) throws Exception{  

	       Configuration conf= new Configuration();

	       FileSystem hdfs = FileSystem.get(URI.create(hdfsUrl),conf);

	       Path path = new Path(oldPath);

	       Path nPath = new Path(newPath);

	       hdfs.close();

	       System.out.println(hdfs.rename(path, nPath));

	     }

	     /**

	      * delete a file 删除一个文件

	      * @param hadoopFile

	      * @return isDeleted

	      */

	     public  static boolean deleteFile(String hadoopFile) throws Exception{

	    	 Configuration conf=new Configuration();

	    	 FileSystem hdfs = FileSystem.get(URI.create(hdfsUrl),conf);

	    	 Path path = new Path(hadoopFile);

	    	 boolean isDeleted = hdfs.delete(path);

	    	 hdfs.close();

	    	 return isDeleted;

	    }

	     /**

	      * upload a local file 上传文件

	      * @param localPath

	      * @param hadoopPath

	      * @return

	      */

	     public static void uploadLocalFile(String localPath,String hadoopPath) throws Exception{  

	       Configuration conf= new Configuration();

	       FileSystem hdfs = FileSystem.get(URI.create(hdfsUrl),conf);

	       Path src = new Path(localPath);

	       Path dst = new Path(hadoopPath);

	       hdfs.copyFromLocalFile(src, dst);

	       hdfs.close();

	     }

	     /**

	      * 读取文件于字节缓冲数组

	      * @param hadoopFile

	      * @return buffer

	      */

	     public static byte[] readFile(String hadoopFile) throws Exception

	     {

	         Configuration conf = new Configuration();

	         FileSystem hdfs = FileSystem.get(URI.create(hdfsUrl),conf);       

	         Path path = new Path(hadoopFile);

	         if ( hdfs.exists(path) )

	         {

	             FSDataInputStream in = hdfs.open(path);

	             FileStatus stat = hdfs.getFileStatus(path);       

	             byte[] buffer = new byte[Integer.parseInt(String.valueOf(stat.getLen()))];

	             in.readFully(0, buffer);

	             in.close();

	             hdfs.close();             

	             return buffer;

	         }

	         else

	         {

	             throw new Exception("the file is not found .");

	         }

	     }

	     /**

	      * list files under folder 列出文件夹中所有文件

	      * @param hadoopPath

	      * @return fileString

	      */

	       public static String listFiles(String hadoopPath) throws Exception{ 

	               Configuration conf= new Configuration();

	               FileSystem hdfs = FileSystem.get(URI.create(hdfsUrl),conf);

	               Path dst = new Path("/test");

	               FileStatus[] files = hdfs.listStatus(dst);

	               String fileString = "";

	               for(FileStatus file: files){

	            	   System.out.println(file.getPath().toString());

	            	   fileString += file.getPath().toString() + " ";

	               }

	               hdfs.close();

	               return fileString;

	       }

	       /**

	        * list block info of file 查找文件所在的数据块

	        * @param hadoopPath

	        * @return blockString

	        */

	       public static String getBlockInfo(String hadoopPath) throws Exception{  

	         Configuration conf= new Configuration();

	         FileSystem hdfs = FileSystem.get(URI.create(hdfsUrl),conf);

	         Path dst = new Path(hadoopPath);

	         FileStatus fileStatus = hdfs.getFileStatus(dst);                                                    

	         BlockLocation[] blkloc=hdfs.getFileBlockLocations(fileStatus,0,fileStatus.getLen()); //查找文件所在数据块

	         String blockString = "";

	         for(BlockLocation loc: blkloc){

	             for(int i=0;i < loc.getHosts().length;i++)

	                 System.out.println(loc.getHosts()[i]);

	  //           	 blockString += loc.getHosts()[i] + " ";

	              }

	         hdfs.close();

	         return blockString;

	          }

	       public  static void main(String[] args) throws Exception

	       {

	    	   //createDir("/test2");

	    	   //deleteDir("/test2");

	    	   //createFile("/test/helloworld.txt");

	    	   //renameFile("/test/a.txt","/test/b.txt");

	    	   //uploadLocalFile("/home/had/in","/test");

	    	   //listFiles("/test");

	    	   //getBlockInfo("/test/b.txt");

	    	   //deleteFile("/test/hellwo.txt");

	    	   /*

	    	   byte[] buffer = readFile("/test/in");

	    	   String out = new String(buffer);

	    	   System.out.println(out);

	    	   */

	       }

	}`

<span style="white-space:pre"></span>