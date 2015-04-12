/**
* Create an object literal with the following key value pairs:
* type: {string} 'Goldfish'
* brand: {string} 'Pepperidge Farm'
* flavor: {string} 'Cheddar'
* count: {number} 2000
* It should be returned directly by the following function
* @return {object} - the object literal
*/

function returnObjectLiteral() {
  //your code here
  return {type: 'Goldfish',
          brand: 'Pepperidge Farm',
          flavor: 'Cheddar',
          count: 2000};
  //end your code
}

/**
* Create a constructor function for a `MessageLog` object.
* @constructor
* @param {string} user - The user associated to the message log
* The string indicating the user should be stored in the user property of the
* object instances.
*
* In addition, the following methods should be
* callable on a MessageLog object:
* logMessage( {string} messageText, {number} direction) - This should log a
* message
* as either being sent or received. A direction of 0 indicates it is a message
* the user sent. A direction of 1 indicates it is a message the user received.
* Behavior for other numbers is undefined.
* getSentMessage({number} n) - returns as a string, the content of the nth most
* recently sent message. To conserve memory, the object should only keep the
* last 5 message. n=0 retrieves the most recent n=4 retrieves the least recent
* of the 5.
* totalSent() - returns an integer indicating the total number of messages sent
* totalReceived() - returns an integer indicating the total number of messages
* received
*/

//your code here

// initialize all properties in the constructor
function MessageLog(user) {
  this.user = user;
  this.sentCount = 0;         // total number of messages sent
  this.receivedCount = 0;     // total number of messages received
  this.sentMessages = [];     // stores sent messages (up to 5)
  this.receivedMessages = []; // stores received messages (up to 5)
}

// attach methods to the prototype, per the Google JS style guide
/**
* Logs a message in the MessageLog instance.
* @param {string} messageText - text of message to log
* @param {number} direction - 0 for sent message, 1 for received message
*/
MessageLog.prototype.logMessage = function(messageText, direction) {
  if (direction === 0) {
    // add message to end of sentMessages
    this.sentMessages.push(messageText);
    this.sentCount++;
    // remove first element if > 5 elements in array
    if (this.sentMessages.length > 5) {
      this.sentMessages.shift();
    }
  } else if (direction === 1) {
    // add message to end of receivedMessages
    this.receivedMessages.push(messageText);
    this.receivedCount++;
    // remove first element if > 5 elements in array
    if (this.receivedMessages.length > 5) {
      this.receivedMessages.shift();
    }
  }
};

/**
* Gets the nth most recent message from the MessageLog instance.
* @param {number} n - n=0 most recent n=4 least recent
* @return {string} - text of nth most recent sent message
*/
MessageLog.prototype.getSentMessage = function(n) {
  // make sure the message actually exists
  if (this.sentMessages.length >= n) {
    return String(this.sentMessages[this.sentMessages.length - n - 1]);
  }
};

/**
* Gets the total number of sent messages logged in the MessageLog object.
* @return {number} - total number of sent messages logged
*/
MessageLog.prototype.totalSent = function() { return this.sentCount; };

/**
* Gets the total number of received messages logged in the MessageLog object.
* @return {number} - total number of received messages logged
*/
MessageLog.prototype.totalReceived = function() { return this.receivedCount; };
//end your code

/**
* Add a method to the MessageLog prototype:
* lastReceivedMessage() - returns the message text of the last message the user
* received.
*/
//your code here
/**
* Gets the most recent received message logged in the MessageLog object.
* @return {string} - the most recent message or 'undefined' if empty
*/
MessageLog.prototype.lastReceivedMessage = function() {
  // make sure there are received messages to return
  if (this.receivedMessages.length > 0) {
    return String(this.receivedMessages[this.receivedMessages.length - 1]);
  }
};
//end your code

/**
* Create an instance of a `MessageLog` for the user "BlackHatGuy". Have the
* instance receive 3 messages: "foo", "bar" and "baz", received in that order.
* Assign it to the variable myLog.
*/

//your code here
myLog = new MessageLog('BlackHatGuy');
myLog.logMessage('foo', 1);
myLog.logMessage('bar', 1);
myLog.logMessage('baz', 1);
//end your code
