function addCount (Counter) {
  url=$('.title a').attr('href').trim();
  title = $('.title').text().trim();
  var query = new AV.Query(Counter);
  
  //use url as unique idnetfication
  query.equalTo("url",url);
  query.find({
    success: function(results){
      if(results.length > 0) {
        var counter = results[0];
        counter.fetchWhenSave(true); //get recent result
        counter.increment("time");
        counter.save();
      } else {
        var newcounter = new Counter();
        newcounter.set("title", title);
        newcounter.set("url", url);
        newcounter.set("time", 1);
        newcounter.save(null, {
          success: function(newcounter){
            //alert('New object created');
          },
          error: function(newcounter,error){
            console.log('Failed to create');
          }
        });
      }
    },
    error: function(error){
      //find null is not a error
      console.log('Error:'+error.code+" "+error.message);
    }
  });
}