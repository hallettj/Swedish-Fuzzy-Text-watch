var isReady = false;
var callbacks = [];

var alignments = {
  center: 0,
  left:   1,
  right:  2
};

var langs = {
  de: 0,
  en: 1,
  es: 2,
  fr: 3,
  no: 4,
  sv: 5
};

function readyCallback(event) {
  isReady = true;
  var callback;
  while (callbacks.length > 0) {
    callback = callbacks.shift();
    callback(event);
  }
}

function showConfiguration(event) {
  onReady(function() {
    var opts = getOptions();
    var url  = "http://static.sitr.us.s3-website-us-west-2.amazonaws.com/configure-fuzzy-text.html";
    Pebble.openURL(url + "#options=" + opts);
  });
}

function webviewclosed(event) {
  console.log('configuration response: '+ event.response + ' ('+ typeof event.response +')');

  var options = JSON.parse(decodeURIComponent(event.response));
  if (typeof options.invert === 'undefined' &&
      typeof options.text_align === 'undefined' &&
      typeof options.lang === 'undefined') {
    return;
  }

  onReady(function() {
    setOptions(event.response);

    var message = prepareConfiguration(event.response);
    transmitConfiguration(message);

  });
}

// Retrieves stored configuration from localStorage.
function getOptions() {
  return localStorage.getItem("options") || encodeURIComponent("{}");
}

// Stores options in localStorage.
function setOptions(options) {
  localStorage.setItem("options", options);
}

// Takes a string containing serialized JSON as input.  This is the
// format that is sent back from the configuration web UI.  Produces
// a JSON message to send to the watch face.
function prepareConfiguration(serialized_settings) {
  var settings = JSON.parse(decodeURIComponent(serialized_settings));
  return {
    "0": settings.invert ? 1 : 0,
    "1": alignments[settings.text_align],
    "2": langs[settings.lang]
  };
}

// Takes a JSON message as input.  Sends the message to the watch.
function transmitConfiguration(settings) {
  console.log('sending message: '+ JSON.stringify(settings));
  Pebble.sendAppMessage(settings, function(event) {
    // Message delivered successfully
  }, logError);
}

function logError(event) {
  console.log('Unable to deliver message with transactionId='+
              event.data.transactionId +' ; Error is'+ event.error.message);
}

function onReady(callback) {
  if (isReady) {
    callback();
  }
  else {
    callbacks.push(callback);
  }
}

Pebble.addEventListener("ready", readyCallback);
Pebble.addEventListener("showConfiguration", showConfiguration);
Pebble.addEventListener("webviewclosed", webviewclosed);

onReady(function(event) {
  var message = prepareConfiguration(getOptions());
  transmitConfiguration(message);
});
