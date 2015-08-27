<header>
    <nav class="top-bar" data-topbar role="navigation" data-options="is_hover: false">
        <ul class="title-area">
            <li class="name">
                <h1><a href="index.php">Issue Tracker</a></h1>
            </li>
            <li class="toggle-topbar menu-icon">
                <a href="#"><span></span></a>
            </li>
        </ul>
        <section class="top-bar-section">
            <ul class="right">
<?php if (isAdmin($mysqli, $user_id)): ?>
                <li class="divider"></li>
                <li class="has-dropdown">
                    <a href="#"><i class="fi-widget"></i>&nbsp;Admin</a>
                    <ul class="dropdown">
                        <li><a href="languages.php">Languages</a></li>
                        <li><a href="priorities.php">Priorities</a></li>
                        <li><a href="relations.php">Relationships</a></li>
                        <li><a href="statuses.php">Statuses</a></li>
                        <li><a href="users.php">Users</a></li>
                    </ul>
                </li>
<?php endif ?>
                <li class="divider"></li>
                <li><a href="profile.php"><i class="fi-torso"></i>&nbsp;&nbsp;<?php echo htmlspecialchars($first); ?></a></li>
                <li class="divider"></li>
                <li class="has-dropdown">
                    <a href="#"><?php echo htmlspecialchars($ui_lang_name); ?></a>
                    <ul class="dropdown">
<?php foreach ($languages as $lang): ?>
<?php if ($lang["lang_id"] != $ui_lang_id): ?>
                        <li><a href="?<?php echo http_build_query(array_merge($_GET, array("lang"=>$lang["lang_id"]))); ?>"><?php echo htmlspecialchars($lang['name']); ?></a></li>
<?php endif ?>
<?php endforeach ?>
                    </ul>
                </li>
                <li class="divider"></li>
                <li class="has-form"><a href="login.php?action=logout" class="button small radius">Sign Out</a></li>
            </ul>
        </section>
    </nav>
</header>
