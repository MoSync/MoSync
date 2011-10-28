-- phpMyAdmin SQL Dump
-- version 3.4.5
-- http://www.phpmyadmin.net
--
-- Värd: localhost
-- Skapad: 28 okt 2011 kl 13:06
-- Serverversion: 5.5.16
-- PHP-version: 5.3.8

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Databas: `benchmark`
--

-- --------------------------------------------------------

--
-- Tabellstruktur `linpack_testruns`
--

CREATE TABLE IF NOT EXISTS `linpack_testruns` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'unique id',
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'current timestamp',
  `revision` varchar(30) NOT NULL COMMENT 'mosync revision',
  `runtime_id` int(11) NOT NULL,
  `git_hash` varchar(40) NOT NULL,
  `phone_id` int(11) NOT NULL,
  `native_sdk_ver` varchar(50) NOT NULL,
  `mflops` double NOT NULL,
  PRIMARY KEY (`id`),
  KEY `revision` (`revision`),
  KEY `runtime_id` (`runtime_id`),
  KEY `phone_id` (`phone_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Tabellstruktur `opengl_testruns`
--

CREATE TABLE IF NOT EXISTS `opengl_testruns` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP,
  `revision` varchar(30) NOT NULL,
  `runtime_id` int(11) NOT NULL,
  `git_hash` varchar(40) NOT NULL,
  `phone_id` int(11) NOT NULL,
  `native_sdk_ver` varchar(50) NOT NULL,
  `test1` int(11) NOT NULL,
  `test2` int(11) NOT NULL,
  `test3` int(11) NOT NULL,
  `test4` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `revision` (`revision`),
  KEY `runtime_id` (`runtime_id`),
  KEY `phone_id` (`phone_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Tabellstruktur `phones`
--

CREATE TABLE IF NOT EXISTS `phones` (
  `phone_id` int(11) NOT NULL AUTO_INCREMENT,
  `phone_name` varchar(30) NOT NULL,
  PRIMARY KEY (`phone_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Tabellstruktur `runtimes`
--

CREATE TABLE IF NOT EXISTS `runtimes` (
  `runtime_id` int(11) NOT NULL AUTO_INCREMENT,
  `runtime_name` varchar(30) NOT NULL,
  PRIMARY KEY (`runtime_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=7 ;

--
-- Dumpning av Data i tabell `runtimes`
--

INSERT INTO `runtimes` (`runtime_id`, `runtime_name`) VALUES
(1, 'MoSync'),
(2, 'iOS'),
(3, 'Android Dalvik'),
(4, 'Android NDK'),
(5, 'WP7'),
(6, 'JavaScript');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
