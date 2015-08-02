Pebble.addEventListener("ready", function(e){
    console.log("Hello C");
});

Pebble.addEventListener("showConfiguration", function(e){
    Pebble.openURL("https://edwinfinch.github.io/config-ola");
});

Pebble.addEventListener("webviewclosed", function(e){
    console.log("Got response");
    if(e.response){
        Pebble.sendAppMessage(JSON.parse(e.response), function(data){
            console.log("Success: " + e.response);
        }, function(data, error){
            console.log("Error: " + error);
        });
    }
});

function getShortFormFromLang(language){
    switch (language) {
        case "German":
            return "de";
        case "French":
            return "fr";
        case "Italian":
            return "it";
        case "Croatian":
            return "hr";
        case "Polish":
            return "pl";
        case "Portuguese":
            return "pt";
        case "Romanian":
            return "ro";
        case "Spanish":
            return "es";
    }
}

function sendLanguageRequest(phrase, language){
    var api_key = "trnsl.1.1.20150802T190308Z.d01a465d6a01dd39.0ecdb418946895861a7a0efce34178308e25062a";
    var root_url = "https://translate.yandex.net/api/v1.5/tr.json/translate?key=" + api_key + "&lang=en-" + getShortFormFromLang(language) + "&text=" + encodeURIComponent(phrase);
    var response;
  var req = new XMLHttpRequest();
  req.open('GET', root_url, true);
  console.log("Getting new data from: " + root_url);
  req.onload = function(e) {
    if (req.readyState == 4) {
      if(req.status == 200) {
          console.log("response " + req.responseText);
      	response = JSON.parse(req.responseText);
          Pebble.sendAppMessage({
            "translation":response.text[0]
		  });
      } else {
			console.log("Error: could not connect! " + JSON.parse(e.responseText));
      }
    }
  };
  req.send();
}

Pebble.addEventListener("appmessage", function(e){
    switch(e.payload.MESSAGE_TYPE){
        case 0:
            sendLanguageRequest(e.payload.phrase, e.payload.language);
            break;
    }
});
