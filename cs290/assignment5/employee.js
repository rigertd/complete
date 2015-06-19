/**
 * @file A collection of functions and classes for managing employees in 
 *       a company payroll system.
 * @author David Rigert
 * @version 1.0.0
 * @copyright 2015 David Rigert
 * @license The MIT License (MIT)
 */

if (typeof Payroll == "undefined") {
    /**
     * Namespace for the payroll system.
     * @namespace
     */
    var Payroll = {};
}

/**
 * Adds the specified employee to the payroll system.
 * @param {Payroll.Employee} emp    - The employee to add to the system.
 */
Payroll.addEmployee = function (emp) {};

/**
 * Represents an individual employee to be managed in the payroll system.
 * @class
 * @property {string} firstName     - The employee's first name.
 * @property {string} middleInit    - The employee's middle initial.
 * @property {string} lastName      - The employee's last name.
 * @property {string} ssn           - The employee's social security number.
 * @property {Date} dateHired       - The date the employee was hired.
 */
Payroll.Employee = function() {
    /**
     * The unique employee ID.
     * @private
     */
    var id;
    
    /**
     * The employee's date of birth.
     * @protected
     */
    this._dob = new Date();
};

/**
 * This method gets the full name of the employee. If the <var>includeMiddle</var>
 * parameter is set to <code>true</code>, the middle initial is included.
 * Otherwise, only the first and last name are included.
 *  
 * @summary Gets the full name of the employee.
 * @param {boolean} includeMiddle   - Whether to include the middle initial.
 * @returns {string} Returns the full name of the employee.
 */
Payroll.Employee.prototype.getFullName = function (includeMiddle) {};

/**
 * Gets the amount that the employee will receive based on the specified rating.
 * @param {number} rating - The employee rating for the current reporting period.
 * @returns {number} Returns the amount of the employee's bonus payment. 
 */
Payroll.Employee.prototype.getBonus = function (rating) {};

/**
 * Represents a manager to be managed in the payroll system.
 * @class
 * @augments Employee
 */
Payroll.Manager = function() {};
Payroll.Manager.prototype = new Employee();

/**
 * Gets the amount that the manager will receive based on the specified rating.
 * @param {number} rating - The manager rating for the current reporting period.
 * @returns {number} Returns the amount of the manager's bonus payment. 
 */
Payroll.Manager.prototype.getBonus = function (rating) {};

/**
 * Gets the manager's team of direct reports.
 * @returns {Payroll.Manager~Team} Returns information on the manager's team and direct reports. 
 */
Payroll.Manager.prototype.getTeam = function () {};

/**
 * The reporting structure of a manager.
 * @typedef {Object} Payroll.Manager~Team
 * @property {string} teamName          - Specifies the name of the manager's team.
 * @property {Array.<Payroll.Employee>} reports - Specifies the members on the manager's team.
 */
 