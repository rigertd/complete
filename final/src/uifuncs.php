<?php
/**
 * Stores database access functions common to all pages.
 * Caches values that do not change often.
 * Assumes the session is already started.
 * User: David Rigert
 * Date: 7/31/2015
 * Time: 8:46 PM
 */

/*
 * Variables for roles.
 */
$PROJECT = 1;
$UPDATE = 2;
$CREATE = 3;
$TRANSLATE = 4;
$READ = 5;
$role_map = array(
    $PROJECT => "Project Manager",
    $UPDATE => "Updater",
    $CREATE => "Author",
    $TRANSLATE => "Translator",
    $READ => "Reader"
);

function updateIssue($db, $user_id, $lang_id, $issue_id, $priority_id, $status_id, $assignee_id, $due, $completed_on, $proj_id) {
    global $UPDATE;
    global $PROJECT;
    $query = "UPDATE Issues i SET i.priority_id = ?, i.status_id = ?, i.assignee = ?, ".
             "                    i.due_date = ?, i.completed_on = ?, i.proj_id = ? ".
             "WHERE i.issue_id = ? AND ".
             "(EXISTS (SELECT * FROM Users WHERE user_id = $user_id AND admin = 1) ".
             " OR $user_id IN (SELECT user_id FROM Users_Projects WHERE proj_id = ? AND (role = $PROJECT OR role = $UPDATE)));";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "iiissiii", $priority_id, $status_id, $assignee_id, $due, $completed_on, $proj_id, $issue_id, $proj_id);
    executeStatement($stmt);
    return $stmt->affected_rows > 0;
}

function updateIssueText($db, $user_id, $lang_id, $issue_id, $subject, $description, $proj_id) {
    global $UPDATE;
    global $PROJECT;
    $query = "UPDATE Issues_Languages il SET il.user_id = $user_id, il.subject = ?, il.description = ?, i.lang_id = $lang_id ".
             "INNER JOIN Issues i ON il.issue_id = i.issue_id ".
             "INNER JOIN Users_Projects up ON i.proj_id = up.proj_id ".
             "WHERE il.lang_id = $lang_id AND il.issue_id = ? AND ".
             "(EXISTS (SELECT * FROM Users WHERE user_id = $user_id AND admin = 1) ".
             " OR $user_id IN (SELECT user_id FROM Users_Projects WHERE proj_id = ? AND (role = $PROJECT OR role = $UPDATE)));";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "ssii", $subject, $description, $issue_id, $proj_id);
    executeStatement($stmt);
    return $stmt->affected_rows > 0;
}

function addIssueComment($db, $user_id, $lang_id, $issue_id, $comment, $proj_id) {
    global $UPDATE;
    global $PROJECT;
    $query = "INSERT INTO Comments (issue_id, user_id, lang_id) ".
             "SELECT i.issue_id, up.user_id, ? ".
             "FROM Users_Projects up INNER JOIN Issues i ON up.proj_id = i.proj_id ".
             "WHERE up.user_id = $user_id AND i.issue_id = ? AND ".
             "(EXISTS (SELECT * FROM Users WHERE user_id = $user_id AND admin = 1) ".
             " OR $user_id IN (SELECT user_id FROM Users_Projects WHERE proj_id = ? AND (role = $PROJECT OR role = $UPDATE)));";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "iii", $lang_id, $issue_id, $proj_id);
    executeStatement($stmt);
    $comment_id = $stmt->insert_id;
    /* if addition was successful, add text */
    if ($comment_id != null && $stmt->affected_rows > 0) {
        $query = "INSERT INTO Comments_Languages (comment_id, lang_id, description) VALUES (?, ?, ?);";
        $stmt = prepareQuery($db, $query);
        bindParam($stmt, "iis", $comment_id, $lang_id, $comment);
        executeStatement($stmt);
        /* delete comment if text insertion failed */
        if ($stmt->affected_rows < 1) {
            $db->query("DELETE FROM Comments WHERE comment_id = $comment_id");
            return false;
        }
    }
    return $stmt->affected_rows > 0;
}

/**
 * Gets an array of all UI languages in the database. Caches the
 * data in the session.
 * @param mysqli $db    The mysqli database instance.
 * @return array        An array of languages {lang_id, name}
 */
function getLanguages($db) {
    if (isset($_SESSION["languages"])) {
        $languages = json_decode($_SESSION["languages"], JSON_UNESCAPED_UNICODE);
    } else {
        $results = $db->query("SELECT lang_id, name, lang_code, locale_code FROM Languages ORDER BY name;");
        $languages = array();
        while ($row = $results->fetch_assoc()) {
            $languages[] = $row;
        }
        $_SESSION["languages"] = json_encode($languages, JSON_UNESCAPED_UNICODE);
    }
    return $languages;
}

function getPriorities($db) {
    if (isset($_SESSION["priorities"])) {
        $priorities = json_decode($_SESSION["priorities"], JSON_UNESCAPED_UNICODE);
    } else {
        $results = $db->query("SELECT priority_id, name, val FROM Priorities ORDER BY val DESC;");
        $priorities = array();
        while ($row = $results->fetch_assoc()) {
            $priorities[] = $row;
        }
        $_SESSION["priorities"] = json_encode($priorities, JSON_UNESCAPED_UNICODE);
    }
    return $priorities;
}

function getRelationships($db) {
    if (isset($_SESSION["relationships"])) {
        $relationships = json_decode($_SESSION["relationships"], JSON_UNESCAPED_UNICODE);
    } else {
        $results = $db->query("SELECT rel_id, forward_desc, reverse_desc FROM Relationships ORDER BY rel_id;");
        $relationships = array();
        while ($row = $results->fetch_assoc()) {
            $relationships[] = $row;
        }
        $_SESSION["relationships"] = json_encode($relationships, JSON_UNESCAPED_UNICODE);
    }
    return $relationships;
}

function getStatuses($db) {
    if (isset($_SESSION["statuses"])) {
        $statuses = json_decode($_SESSION["statuses"], JSON_UNESCAPED_UNICODE);
    } else {
        $results = $db->query("SELECT status_id, name, percentage FROM Statuses ORDER BY status_id;");
        $statuses = array();
        while ($row = $results->fetch_assoc()) {
            $statuses[] = $row;
        }
        $_SESSION["statuses"] = json_encode($statuses, JSON_UNESCAPED_UNICODE);
    }
    return $statuses;
}

function getProjectUsers($db, $user_id, $project_id) {
    $query = "SELECT DISTINCT u.user_id, u.username, u.first_name, u.last_name ".
        "FROM Users u INNER JOIN Users_Projects up ON u.user_id = up.user_id ".
        "WHERE up.proj_id = ? AND (".
        "EXISTS (SELECT * FROM Users WHERE user_id = $user_id AND admin = 1) ".
        "OR up.proj_id IN (SELECT proj_id FROM Users_Projects WHERE user_id = $user_id)".
        ") ORDER BY u.username";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "i", $project_id);
    executeStatement($stmt);
    $results = $stmt->get_result();
    $users = array();
    while ($row = $results->fetch_assoc()) {
        $users[] = $row;
    }
    return $users;
}

function getProjectUsersRoles($db, $user_id, $project_id) {
    global $PROJECT;
    $query = "SELECT u.user_id, u.username, u.first_name, u.last_name, up.role ".
             "FROM Users u INNER JOIN Users_Projects up ON u.user_id = up.user_id ".
             "WHERE up.proj_id = ? AND (".
                 "EXISTS (SELECT * FROM Users WHERE user_id = $user_id AND admin = 1) ".
                 "OR up.proj_id IN (SELECT proj_id FROM Users_Projects WHERE user_id = $user_id AND role = $PROJECT)".
             ") ORDER BY u.username";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "i", $project_id);
    executeStatement($stmt);
    $results = $stmt->get_result();
    $users = array();
    while ($row = $results->fetch_assoc()) {
        $users[] = $row;
    }
    return $users;
}

function getAllUsers($db) {
    if (isset($_SESSION["users"])) {
        $users = json_decode($_SESSION["users"], JSON_UNESCAPED_UNICODE);
    } else {
        $results = $db->query("SELECT user_id, first_name, last_name, username, email, ui_lang_id, name AS lang_name, admin FROM Users INNER JOIN Languages on ui_lang_id = lang_id ORDER BY user_id;");
        $users = array();
        while ($row = $results->fetch_assoc()) {
            $users[] = $row;
        }
        $_SESSION["users"] = json_encode($users, JSON_UNESCAPED_UNICODE);
    }
    return $users;
}

function getProjects($db, $userId, $sort_col, $sort_dir, $proj_id = NULL) {
    /* set ORDER BY column based on row number */
    switch ($sort_col) {
        case "2":
            $ob = "p.name";
            break;
        case "3":
            $ob = "p.description";
            break;
        default:
            $ob = "p.proj_id";
    }

    $query = "SELECT DISTINCT p.proj_id, p.name, p.description ".
             "FROM Projects p ".
             "INNER JOIN Users_Projects up ON p.proj_id = up.proj_id ".
             "WHERE (EXISTS (SELECT * FROM Users WHERE user_id = ? AND admin = 1) ".
             "OR up.user_id = ?) ".
             (is_null($proj_id) ? "" : "AND p.proj_id = ? ").
             "ORDER BY $ob $sort_dir;";
    $stmt = prepareQuery($db, $query);
    if (is_null($proj_id)) {
        bindParam($stmt, "ii", $userId, $userId);
    } else {
        bindParam($stmt, "iii", $userId, $userId, $proj_id);
    }
    executeStatement($stmt);
    $results = $stmt->get_result();
    $projects = array();
    while ($row = $results->fetch_assoc()) {
        $projects[] = $row;
    }
    return $projects;
}

/**
 * Gets the issues that the current user is allowed to view.
 * @param mysqli $db       The mysqli database instance.
 * @param String $userId   The current user's user_id.
 * @param String $langId  The lang_id of the text to display.
 * @param String $sort_col The column number to order the results by.
 * @param String $sort_dir The sort direction (ASC or DESC).
 * @param String|null $issue_id    The issue to get if looking for a specific issue.
 * @param String|null $assigned_to The assignee of the issues to get. Omit for all issues.
 * @param String|null $project_id  The project of the issues to get. Omit for all projects.
 * @return array All issues the current user has access to {issue_id, subject, status, priority, due_date, assignee_name}
 */
function getIssues($db, $userId, $langId, $sort_col, $sort_dir, $issue_id = NULL, $assigned_to = NULL, $project_id = NULL) {
    /* set ORDER BY column based on row number */
    switch ($sort_col) {
        case "2":
            $ob = "subject";
            break;
        case "3":
            $ob = "status";
            break;
        case "4":
            $ob = "priority";
            break;
        case "5":
            $ob = "assignee_name";
            break;
        case "6":
            $ob = "proj_name";
            break;
        case "7":
            $ob = "i.due_date";
            break;
        default:
            $ob = "i.issue_id";
    }
    $query = "SELECT DISTINCT i.issue_id,".
             "  COALESCE(t_il.subject, s_il.subject) AS subject,".
             "  COALESCE(t_il.description, s_il.description) AS description,".
             "  uu.created_on AS last_update, uu.updated_by,".
             "  CASE WHEN t_il.subject IS NULL THEN 0 ELSE 1 END AS translated,".
             "  i.priority_id, pr.name AS priority, i.status_id, s.name AS status, u.user_id AS assignee_id,".
             "  CONCAT(COALESCE(u.first_name, ''), ' ', COALESCE(u.last_name, '')) AS assignee_name,".
             "  i.proj_id, p.name AS proj_name, i.due_date, i.created_on, ".
             "  CONCAT(COALESCE(cu.first_name, ''), ' ', COALESCE(cu.last_name, '')) AS created_by, i.completed_on ".
             "FROM Issues i ".
             "INNER JOIN Priorities pr ON i.priority_id = pr.priority_id ".
             "INNER JOIN Statuses s ON i.status_id = s.status_id ".
             "INNER JOIN Projects p ON i.proj_id = p.proj_id ".
             "INNER JOIN (SELECT DISTINCT user_id, proj_id ".
             "            FROM Users_Projects ".
             "            WHERE user_id = ? ".
             "            OR EXISTS (SELECT * FROM Users WHERE user_id = ? AND admin = 1)) up ON p.proj_id = up.proj_id ".
             "INNER JOIN Users cu ON i.created_by = cu.user_id ".
             "LEFT JOIN Users u ON i.assignee = u.user_id ".
             "LEFT JOIN Issues_Languages t_il ON i.issue_id = t_il.issue_id AND t_il.lang_id = ? ".
             "LEFT JOIN Issues_Languages s_il ON i.issue_id = s_il.issue_id AND s_il.lang_id = i.lang_id ".
             "LEFT JOIN (SELECT ud.user_id, CONCAT(COALESCE(uu.first_name, ''), ' ', COALESCE(uu.last_name, '')) AS updated_by, ud.created_on, ud.issue_id ".
             "           FROM Updates ud INNER JOIN Users uu ON ud.user_id = uu.user_id) AS uu ".
             "           ON i.issue_id = uu.issue_id ".
             "WHERE 1=1 ". //hack for conditional code below
             (is_null($assigned_to) ? "" : "AND i.assignee = ? ").
             (is_null($issue_id) ? "" : "AND i.issue_id = ? ").
             (is_null($project_id) ? "" : "AND i.proj_id = ? ").
             "ORDER BY $ob $sort_dir;";
    //echo $query;
    $stmt = prepareQuery($db, $query);
    if (!is_null($assigned_to) && !is_null($issue_id) && !is_null($project_id)) {
        bindParam($stmt, "iiiiii", $userId, $userId, $langId, $assigned_to, $issue_id, $project_id);
    } else if (!is_null($assigned_to) && !is_null($issue_id)) {
        bindParam($stmt, "iiiii", $userId, $userId, $langId, $assigned_to, $issue_id);
    } else if (!is_null($assigned_to) && !is_null($project_id)) {
        bindParam($stmt, "iiiii", $userId, $userId, $langId, $assigned_to, $project_id);
    } else if (!is_null($issue_id) && !is_null($project_id)) {
        bindParam($stmt, "iiiii", $userId, $userId, $langId, $issue_id, $project_id);
    } else if (!is_null($assigned_to)) {
        bindParam($stmt, "iiii", $userId, $userId, $langId, $assigned_to);
    } else if (!is_null($issue_id)) {
        bindParam($stmt, "iiii", $userId, $userId, $langId, $issue_id);
    } else if (!is_null($project_id)) {
        bindParam($stmt, "iiii", $userId, $userId, $langId, $project_id);
    } else {
        bindParam($stmt, "iii", $userId, $userId, $langId);
    }
    executeStatement($stmt);
    $results = $stmt->get_result();
    $issues = array();
    while ($row = $results->fetch_assoc()) {
        $issues[] = $row;
    }
    return $issues;
}

/**
 * Gets the name of a language based on the language ID.
 * @param $db
 * @param $langId
 * @return null
 */
function getLangName($db, $langId) {
    $stmt = prepareQuery($db, "SELECT name FROM Languages WHERE lang_id = ?;");
    bindParam($stmt, "i", $langId);
    executeStatement($stmt);
    $result = getSingleResult($stmt);
    return isset($result)? $result['name'] : NULL;
}

/**
 * Sets the interface language for the specified user.
 * @param $db
 * @param $userId
 * @param $langId
 * @return bool
 */
function setUserLang($db, $userId, $langId) {
    $stmt = prepareQuery($db, "UPDATE Users SET ui_lang_id = ? WHERE user_id = ?;");
    bindParam($stmt, "ii", $langId, $userId);
    executeStatement($stmt);
    return $stmt->affected_rows > 0;
}

/**
 * Handles a GET request to update the interface language.
 * @param $db
 * @param $userId
 */
function handleLangRequest($db, $userId) {
    if (setUserLang($db, $userId, $_REQUEST['lang'])) {
        $ui_lang_name = getLangName($db, $_REQUEST['lang']);
        $ui_lang_id = $_REQUEST['lang'];
        $_SESSION['ui_lang_id'] = $ui_lang_id;
        $_SESSION['ui_lang_name'] = $ui_lang_name;
    }
    // redirect to remove URL parameter
    $url = $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'];
    $url = strtok($url, '?');
    $params = $_GET;
    unset($params['lang']);
    unset($_SESSION['users']);
    $url_param = http_build_query($params);
    if ($url_param) {
        header("Location: https://{$url}?{$url_param}");
    } else {
        header("Location: https://{$url}");
    }
    die();
}

/**
 * Checks whether a user is an admin in any project.
 * @param $db
 * @param $userId
 * @return bool
 */
function isAdmin($db, $userId) {
    $query = "SELECT admin FROM Users WHERE user_id = ?";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "i", $userId);
    executeStatement($stmt);
    $result = $stmt->get_result();
    $admin = $result->fetch_assoc();
    return $admin['admin'] == 1;
}

/**
 * Checks whether a user has the Project Manager role in a project.
 * Checks if the user is PM in any project if $projId is NULL.
 * @param $db
 * @param $userId
 * @param $projId
 * @return bool
 */
function isProjectManager($db, $userId, $projId = NULL) {
    global $PROJECT;
    $query = "SELECT role FROM Users_Projects WHERE role = $PROJECT AND user_id = ? ".
            (is_null($projId) ? "" : "AND proj_id = ?");
    $stmt = prepareQuery($db, $query);
    if (is_null($projId)) {
        bindParam($stmt, "i", $userId);
    } else {
        bindParam($stmt, "ii", $userId, $projId);
    }
    executeStatement($stmt);
    $result = $stmt->get_result();
    $role = $result->fetch_assoc();
    return $role['role'] == $PROJECT;
}

/**
 * Checks whether a user has the Author role in a project.
 * Checks if the user is Author in any project if $projId is NULL.
 * @param $db
 * @param $userId
 * @param $projId
 * @return bool
 */
function isAuthor($db, $userId, $projId = NULL) {
    global $CREATE;
    $query = "SELECT role FROM Users_Projects WHERE role = $CREATE AND user_id = ? ".
        (is_null($projId) ? "" : "AND proj_id = ?");
    $stmt = prepareQuery($db, $query);
    if (is_null($projId)) {
        bindParam($stmt, "i", $userId);
    } else {
        bindParam($stmt, "ii", $userId, $projId);
    }
    executeStatement($stmt);
    $result = $stmt->get_result();
    $role = $result->fetch_assoc();
    return $role['role'] == $CREATE;
}

function getProjectRoles($db, $userId, $projId) {
    $query = "SELECT role FROM Users_Projects WHERE user_id = ? AND proj_id = ?";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "ii", $userId, $projId);
    executeStatement($stmt);
    $results = $stmt->get_result();
    $roles = array();
    while ($row = $results->fetch_assoc()) {
        $roles[$row['role']] = $row['role'];
    }
    return $roles;
}

/**
 * Gets any outdated translation for a specific issue.
 * @param mysqli $db      The mysqli instance.
 * @param int $issueId    The issue to check the translation for.
 * @param int $langId     The language ID of the translation to check.
 * @return array|null Any out-of-date translation for the issue. {issue_id, t_lang_id, t_lang_name}
 */
function getOutdatedIssueTrans($db, $issueId, $langId) {
    $query = "SELECT i.issue_id, t_il.lang_id AS t_lang_id, l.name AS t_lang_name ".
             "FROM Issues i ".
             "INNER JOIN Issues_Languages s_il ON i.issue_id = s_il.issue_id AND i.lang_id = s_il.lang_id ".
             "INNER JOIN Issues_Languages t_il ON i.issue_id = t_il.issue_id AND i.lang_id <> t_il.lang_id ".
             "INNER JOIN Languages l ON t_il.lang_id = l.lang_id ".
             "WHERE t_il.last_update < s_il.last_update AND i.issue_id = ? AND t_il.lang_id = ?";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "ii", $issueId, $langId);
    executeStatement($stmt);
    $result = $stmt->get_result();
    return $result->fetch_assoc();
}

