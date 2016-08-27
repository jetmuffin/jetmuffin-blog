title: nginx 配置图片缓存服务器，nginx.conf的配置
tags: [Nginx,缓存]
categories: 设计开发
date: 2014-11-19 04:38:01
---

<span style="white-space:pre"></span>最近在搞云计算，弄了一个图片缓存服务器服务器，搭了一阵子，做个笔记。

<span style="white-space:pre"></span>教程转自<span style="font-family:Helvetica,Tahoma,Arial,sans-serif; line-height:18px; text-align:center">**Hermit_Macx，**</span><span style="font-family:Helvetica,Tahoma,Arial,sans-serif; line-height:18px; text-align:center">感谢作者，地址[http://hermit-macx.iteye.com/blog/1697375](http://hermit-macx.iteye.com/blog/1697375)</span>

<span style="white-space:pre"></span>PS：proxy_cache_purge 需要另外下载包，不然报错，先注释了把

<!--more-->

<span style="white-space:pre"></span>nginx.conf:

<span style="white-space:pre"></span><pre name="code" class="delphi">user  had;  
worker_processes  8;  

error_log  /usr/local/nginx/nginx8011/logs/error.log crit;  

pid  /usr/local/nginx/nginx8011/logs/nginx.pid;  

events {  
    use epoll;  
    worker_connections  65535;  
}  

http {  
    include       mime.types;  
    default_type  application/octet-stream;  

    charset  utf-8;            
    server_names_hash_bucket_size 128;     
    client_header_buffer_size 32k;     
    large_client_header_buffers 4 32k;     
    client_max_body_size 300m;            
    sendfile on;     
    tcp_nopush     on;  

    keepalive_timeout 60;  
    tcp_nodelay on;      
    client_body_buffer_size  512k;     
    proxy_connect_timeout    5;     
    proxy_read_timeout       60;     
    proxy_send_timeout       5;     
    proxy_buffer_size        16k;     
    proxy_buffers            4 64k;     
    proxy_busy_buffers_size 128k;     
    proxy_temp_file_write_size 128k;      

    gzip on;     
    gzip_min_length  1k;     
    gzip_buffers 4 16k;     
    gzip_http_version 1.1;     
    gzip_comp_level 2;     
    gzip_types text/plain application/x-javascript text/css application/xml;     
    gzip_vary on;  

    #注：images_temp和images_cache指定的路径必须在同一分区  
    proxy_temp_path   /usr/local/nginx/nginx8011/html/tmp;  
    #设置Web缓存区名称为cache_one，内存缓存空间大小为200MB，1天没有被访问的内容自动清除，硬盘缓存空间大小为30GB。  
    proxy_cache_path  /usr/local/nginx/nginx8011/html/cache  levels=1:2   keys_zone=cache_one:200m inactive=1d max_size=30g;  

    server {  
        listen       80;  
        server_name  localhost;  
        location / {  
                proxy_cache cache_one;  
                #对不同的HTTP状态码设置不同的缓存时间  
                proxy_cache_valid  200 304 12h;  
                #以域名、URI、参数组合成Web缓存的Key值，Nginx根据Key值哈希，存储缓存内容到二级缓存目录内  
                proxy_cache_key $host$uri$is_args$args;  
                proxy_set_header Host  $host;  
                proxy_set_header X-Forwarded-For  $remote_addr;  
                proxy_pass http://localhost:8080;  
            log_format cache &#39;***$time_local &#39;  &#39;***$upstream_cache_status &#39;  &#39;***Cache-Control: $upstream_http_cache_control &#39; &#39;***Expires: $upstream_http_expires &#39; &#39;***"$request" ($status) &#39; &#39;***"$http_user_agent" &#39;;  
            access_log  /usr/local/nginx/nginx8011/logs/cache.log cache;  
                expires      1d;  
                }  

            #用于清除缓存，假设一个URL为http://localhost/test.txt，通过访问http://localhost/purge/test.txt就可以清除该URL的缓存。  
    location ~ /purge(/.*) {  
                #设置只允许指定的IP或IP段才可以清除URL缓存。  
            allow       127.0.0.1;  
            allow       192.168.0.0/16;  
            deny    all;  
            #proxy_cache_purge    cache_one   $host$1$is_args$args;  
    }   

            #扩展名以.php、.jsp、.cgi结尾的动态应用程序不缓存。  
    location ~ .*\.(php|jsp|cgi)?$ {  
                proxy_set_header Host  $host;  
                proxy_set_header X-Forwarded-For  $remote_addr;  
                proxy_pass http://localhost:8080;  
        }  
        access_log  off;  
    }  

    server{  
    listen 8080;   
            server_name localhost;   

        location / {
            root   html;
            index  index.html index.htm;
        }
        access_log off;  
     }  
}  </pre>