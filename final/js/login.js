/**
 * @file This file contains the functions and symbols required by the login
 *       and sign-up forms for the Child Growth Tracker website.
 * @author David Rigert [rigertd@onid.oregonstate.edu]
 * @copyright 2015 David Rigert
 */

window.onload = function() {
  /* sign up form elements */
  var suFirst = document.getElementById('signup_first');
  var suLast = document.getElementById('signup_last');
  var suEmail = document.getElementById('signup_email');
  var suPw1 = document.getElementById('signup_password1');
  var suPw2 = document.getElementById('signup_password2');
  var suButton = document.getElementById('signup_button');

  /* login form button */
  var lButton = document.getElementById('login_button');

  /* event handling */
  suFirst.onblur = GrowthTracker.validateElement;
  suLast.onblur = GrowthTracker.validateElement;
  suEmail.onkeyup = GrowthTracker.validateElement;
  suEmail.onblur = GrowthTracker.SignUp.checkEmail;
  suPw1.onblur = GrowthTracker.validateElement;
  suPw2.onkeyup = GrowthTracker.SignUp.validatePassword;

  /* validate form elements and matching password fields */
  suButton.onclick = function() {
    return GrowthTracker.validateForm('signup')
      && GrowthTracker.SignUp.validatePassword()
      && GrowthTracker.SignUp.emailAvailable;
  };

  /* validate username and password */
  lButton.onclick = function() {
    return GrowthTracker.validateForm('login') && GrowthTracker.Login.authenticate();
  };
};

if (typeof GrowthTracker == 'undefined') {
  /**
   * The <code>GrowthTracker</code> namespace contains all of the symbols
   * and functions required for the Baby Growth Tracker website.
   * @namespace
   */
  var GrowthTracker = {
    /**
     * The <code>SignUp</code> namespace contains the functions specific to
     * the account creation form.
     * @namespace
     */
    SignUp: {
      /**
       * Indicates whether the current email address is available or in use.
       */
      emailAvailable: true
    },
    /**
     * The <code>Login</code> namespace contains functions specific to
     * the login form.
     * @namespace
     */
    Login: {}
  };
}

/**
 * Adds a validation message to the validation div if it does not already exist.
 * @param {string} text - The text of the message to display.
 * @param {string} id   - The ID to assign to the message.
 */
GrowthTracker.addValidationMessage = function(id, text) {
  var valDiv = document.getElementById('validation_message');
  var msg = document.createElement('p');
  valDiv.className = 'alert alert-danger';
  msg.id = id;
  msg.textContent = text;
  if (!document.getElementById(id)) {
    valDiv.appendChild(msg);
  }
};

/**
 * Removes a validation message from the validation div.
 * @param {string} id - The ID of the validation message to remove.
 */
GrowthTracker.removeValidationMessage = function(id) {
  var valDiv = document.getElementById('validation_message');
  var msg = document.getElementById(id);
  if (msg) {
    valDiv.removeChild(msg);
  }
  if (!valDiv.hasChildNodes()) {
    valDiv.className = '';
  }
};

/**
 * Checks whether the sign-up email address is already in use.
 * Shows an error if the email is in use.
 */
GrowthTracker.SignUp.checkEmail = function() {
  var email = document.getElementById('signup_email');
  var xhr = new XMLHttpRequest();
  var url = 'login.php?action=email&signup_email=' + encodeURIComponent(email.value);

  xhr.onreadystatechange = function() {
    if (this.readyState === 4) {
      var result = this.responseText;
      if (result === 'false') {
        /* email not already in use */
        GrowthTracker.hideError(email);
        GrowthTracker.SignUp.emailAvailable = true;
        GrowthTracker.removeValidationMessage('emailInUse');
      } else {
        /* email already in use -- show error */
        GrowthTracker.showError(email);
        GrowthTracker.SignUp.emailAvailable = false;
        GrowthTracker.addValidationMessage('emailInUse', 'That e-mail address is already in use.');
      }
    }
  };

  xhr.open('GET', url);
  xhr.send();
};

/**
 * Authenticates the login credentials via <code>XMLHttpRequest</code> with
 * the <code>action</code> parameter set to <code>auth</code>.
 * @returns {boolean} Returns false to block form submission.
 */
GrowthTracker.Login.authenticate = function() {
  var form = document.getElementById('login');
  var email = document.getElementById('email');
  var pw = document.getElementById('password');
  var btn = document.getElementById('login_button');
  var xhr = new XMLHttpRequest();
  var url = 'login.php';
  var params = 'action=auth&email=' + encodeURIComponent(email.value)
             + '&password=' + encodeURIComponent(pw.value);

  /**
   * Determines whether the credentials are correct.
   * Redirects to index.php if correct, otherwise displays error message.
   * @listens event:onreadystatechange
   */
  xhr.onreadystatechange = function() {
    if (this.readyState === 4) {
      /* re-enable sign-in button */
      btn.disabled = false;
      /* get the response text */
      var result = this.responseText;
      if (result === 'true') {
        /* authentication succeeded */
        form.submit();
      } else {
        /* authentication failed */
        alert('Invalid login credentials');
        email.value = '';
        pw.value = '';
      }
    }
  };
  /* disable sign-in button to prevent multiple clicks */
  btn.disabled = true;

  xhr.open('POST', url);
  xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
  xhr.setRequestHeader('Content-length', String(params.length));
  xhr.setRequestHeader('Connection', 'close');
  xhr.send(params);

  /* stop form submission */
  return false;
};

/**
 * Applies error formatting to the parent of the specified element.
 * @param {object} el - The child of the parent to apply the formatting to.
 */
GrowthTracker.showError = function(el) {
  var parent = el.parentNode;
  if (parent.className.indexOf('has-error') == -1) {
    parent.className += ' has-error';
  }
};

/**
 * Removes error formatting from the parent of the specified element.
 * @param {object} el - The child of the parent to remove the formatting from.
 */
GrowthTracker.hideError = function(el) {
  var parent = el.parentNode;
  if (parent.className.indexOf('has-error') != -1) {
    var re = /\s?\bhas-error\b/;
    parent.className = parent.className.replace(re, '');
  }
};

/**
 * Verifies the validity state of all of elements in the specified <var>formId</var>.
 * Applies error formatting if invalid, removes it if valid.
 * @param {string} formId - The <code>id</code> of the form to validate.
 * @returns {boolean} Whether all elements on the form are valid.
 */
GrowthTracker.validateForm = function(formId) {
  var elements = document.getElementById(formId).elements;
  var valid = true;
  for (var i = 0, len = elements.length; i < len; i++) {
    if (elements[i].validity && !elements[i].validity.valid) {
      valid = false;
      GrowthTracker.showError(elements[i]);
    } else {
      GrowthTracker.hideError(elements[i]);
    }
  }
  return valid;
};

/**
 * Verifies the validity state of the calling form element.
 * Applies error formatting if invalid, removes it if valid.
 */
GrowthTracker.validateElement = function() {
  if (this.validity && !this.validity.valid) {
    GrowthTracker.showError(this);
    GrowthTracker.addValidationMessage(this.id + '_msg', this.title + ' is missing or invalid.');
  } else {
    GrowthTracker.hideError(this);
    GrowthTracker.removeValidationMessage(this.id + '_msg');
  }
};

/**
 * Determines whether the text entered in both password fields
 * on the sign-up form matches.
 * @returns {boolean} Whether the password fields match.
 */
GrowthTracker.SignUp.validatePassword = function() {
  var pw1 = document.getElementById('signup_password1');
  var pw2 = document.getElementById('signup_password2');

  if (pw1.value && pw1.value != pw2.value) {
    /* apply error style if passwords do not match */
    GrowthTracker.showError(pw2);
    GrowthTracker.addValidationMessage('pwMatch', 'The passwords do not match.');
    return false;
  } else {
    /* remove error style if passwords match */
    GrowthTracker.hideError(pw2);
    GrowthTracker.removeValidationMessage('pwMatch');
    return true;
  }
};