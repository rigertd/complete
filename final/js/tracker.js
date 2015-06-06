/**
 * @file This file contains the common functions and symbols required by
 *       the Baby Growth Tracker website.
 * @author David Rigert [rigertd@onid.oregonstate.edu]
 * @copyright 2015 David Rigert
 */

if (typeof GrowthTracker == 'undefined') {
  /**
   * The <code>GrowthTracker</code> namespace contains all of the symbols
   * and functions required for the Baby Growth Tracker website.
   * @namespace
   */
  var GrowthTracker = { };
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
    if (elements[i] == 'SELECT') {
      if (!elements[i].options[elements[i].selectedIndex].value) {
        valid = false;
        GrowthTracker.showError(elements[i]);
      }
      else {
        GrowthTracker.hideError(elements[i]);
      }
    } else if (elements[i].validity && !elements[i].validity.valid) {
      valid = false;
      GrowthTracker.showError(elements[i]);
    } else {
      GrowthTracker.hideError(elements[i]);
    }
  }
  return valid;
};

/**
 * Clears all content, error formatting, and validation messages for the specified form.
 * @param {object} form - The form to clear.
 */
GrowthTracker.clearForm = function(form) {
  form.reset();
  for (var i = 0, len = form.elements.length; i < len; i++) {
    GrowthTracker.hideError(form.elements[i]);
    GrowthTracker.removeValidationMessage(form.elements[i].id + '_msg');
  }
};

/**
 * Verifies the validity state of the calling form element.
 * Applies error formatting if invalid, removes it if valid.
 * @param {Object} el     - The element to verify.
 * @returns {boolean} Whether the select element has valid input.
 */
GrowthTracker.validateElement = function(el) {
  if (!el) el = this;
  else if (el.srcElement) el = el.srcElement;
  if (el.validity && !el.validity.valid) {
    GrowthTracker.showError(el);
    GrowthTracker.addValidationMessage(el.id + '_msg', el.title + ' is missing or invalid.');
    return false;
  } else {
    GrowthTracker.hideError(el);
    GrowthTracker.removeValidationMessage(el.id + '_msg');
  }
  return true;
};

/**
 * Verifies the validity of a <code>select</code> form element.
 * Applies error formatting if selected value is blank.
 * @param {Object} obj     - The select element to verify.
 * @param {string} message - The message to display if invalid.
 * @returns {boolean} Whether the select element has valid input.
 */
GrowthTracker.validateSelect = function(obj, message) {
  if (!obj.options[obj.selectedIndex].value) {
    GrowthTracker.showError(obj);
    GrowthTracker.addValidationMessage(obj.id + '_msg', message);
    return false;
  } else {
    GrowthTracker.hideError(obj);
    GrowthTracker.removeValidationMessage(obj.id + '_msg');
  }
  return true;
};

/**
 * Toggles the display CSS style of the specified node between none and normal.
 * @param {Object} node - The node to verify.
 */
GrowthTracker.toggleVisible = function(node) {
  if (node.style.display == 'none') node.style.display = '';
  else node.style.display = 'none';
};