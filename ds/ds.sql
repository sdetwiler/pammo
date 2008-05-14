create database if not exists ds;
use ds;

# The user table stores user accounts
CREATE TABLE `user` 
(
  `id` int(4) unsigned NOT NULL auto_increment,
  `username` VARCHAR(64) NOT NULL UNIQUE,
  `password` VARCHAR(64) NOT NULL,
  PRIMARY KEY (`id`)
) TYPE=InnoDB CHARSET='utf8';

# Adds the default guest user.
##INSERT INTO user VALUES(0,"", "");





# The session table tracks user sessions
CREATE TABLE `session`
(
  `id` int(8) NOT NULL UNIQUE,
  `uid` int(4) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) TYPE=InnoDB;



# DataStore Objects.

CREATE TABLE `object` 
(
  `id` int(4) unsigned NOT NULL auto_increment,
  `oid` int(4) unsigned NOT NULL,
  `createdTime` datetime NOT NULL,
  `modifiedTime` datetime NOT NULL,
  `data` TEXT NOT NULL default '',

   PRIMARY KEY  (`id`)
) TYPE=InnoDB CHARSET='utf8'; 

# DataStore ObjectTemplates

# CREATE TABLE `objecttemplate` 
# (
#   `oid` int(4) unsigned NOT NULL,
#   `createdTime` datetime NOT NULL,
#   `modifiedTime` datetime NOT NULL,
#   `data` TEXT NOT NULL default '',
# 
#   PRIMARY KEY (`id`)
# ) TYPE=InnoDB CHARSET='utf8';