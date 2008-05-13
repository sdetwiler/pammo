use ds;

CREATE TABLE `objecttemplate` (
  `oid` int(4) unsigned NOT NULL,
  `createdTime` datetime NOT NULL,
  `modifiedTime` datetime NOT NULL,
  `data` TEXT NOT NULL default '',

  PRIMARY KEY (`oid`)
) TYPE=InnoDB CHARSET='utf8';