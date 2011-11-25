-- phpMyAdmin SQL Dump
-- version 3.4.5
-- http://www.phpmyadmin.net
--
-- Värd: localhost
-- Skapad: 25 nov 2011 kl 12:23
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
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=26 ;

--
-- Dumpning av Data i tabell `linpack_testruns`
--

INSERT INTO `linpack_testruns` (`id`, `timestamp`, `revision`, `runtime_id`, `git_hash`, `phone_id`, `native_sdk_ver`, `mflops`) VALUES
(1, '2011-11-03 08:30:38', '1', 3, '1234', 2, '2', 12.3),
(2, '2011-11-03 08:31:34', '1', 3, '1234', 2, '', 12.3),
(3, '2011-11-03 08:46:50', '1', 0, '1234', 0, '', 12.3),
(4, '2011-11-03 08:48:28', '1', 0, '1234', 0, '', 12.3),
(5, '2011-11-03 08:48:43', '1', 0, '1234', 1, '', 12.3),
(6, '2011-11-03 09:49:44', '1', 1, '1234', 2, '', 12.3),
(7, '2011-11-03 09:50:41', '1', 1, '1234', 3, '', 12.3),
(8, '2011-11-03 09:51:02', '1', 1, '1234', 3, '', 12.3),
(9, '2011-11-03 11:44:38', '1337', 1, '981723987', 3, '2', 12.8),
(10, '2011-11-03 11:44:44', '1337', 1, '981723987', 3, '2', 12.8),
(11, '2011-11-03 11:45:55', '1337', 1, '981723987', 3, '2', 100),
(12, '2011-11-11 09:30:50', '1337', 1, '987123ab', 3, '2', 5.526492),
(13, '2011-11-11 10:51:59', '1337', 1, '987123ab', 3, '2', 0),
(14, '2011-11-11 10:54:02', '1337', 1, '987123ab', 3, '2', 0),
(15, '2011-11-11 10:56:10', '1337', 1, '987123ab', 3, '2', 5.56745),
(16, '2011-11-11 13:17:52', '1337', 1, '981723987', 3, '2', 12.8),
(17, '2011-11-11 13:20:54', '1337', 1, '981723987', 3, '2', 12.8),
(18, '2011-11-11 13:43:30', '1337', 1, '987123ab', 3, '\n2', 5.559705),
(19, '2011-11-11 16:11:52', '1337', 1, '981723987', 3, '\n2', 1.8),
(20, '2011-11-11 16:12:59', '1337', 3, '981723987', 3, '\n2', 1.8),
(21, '2011-11-11 16:13:40', '1337', 1, '987123ab', 3, '\n2', 5.473487),
(22, '2011-11-11 16:24:18', '1337', 3, '981723987', 3, '\n2', 1.5146714261896888),
(23, '2011-11-25 10:13:09', '1337', 1, '987123ab', 3, '\n2', 5.611161),
(24, '2011-11-25 10:19:39', '1337', 1, '987123ab', 3, '\n2', 5.648668),
(25, '2011-11-25 10:55:21', '1337', 1, '987123ab', 3, '\n2', 5.337);

-- --------------------------------------------------------

--
-- Tabellstruktur `membench_testruns`
--

CREATE TABLE IF NOT EXISTS `membench_testruns` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP,
  `revision` varchar(30) NOT NULL,
  `runtime_id` int(11) NOT NULL,
  `git_hash` varchar(40) NOT NULL,
  `phone_id` int(11) NOT NULL,
  `native_sdk_ver` varchar(50) NOT NULL,
  `alloc_str_10` double NOT NULL,
  `alloc_str_100` double NOT NULL,
  `alloc_void_1` double NOT NULL,
  `alloc_void_100` double NOT NULL,
  `alloc_void_1000` double NOT NULL,
  `alloc_dummy` double NOT NULL,
  `alloc_dummy_struct` double NOT NULL,
  `alloc_dummy_mix` double NOT NULL,
  `access_array` double NOT NULL,
  `access_vector` double NOT NULL,
  `add_vector` double NOT NULL,
  `access_dummy` double NOT NULL,
  `access_dummy_struct` double NOT NULL,
  `access_dummy_mix` double NOT NULL,
  PRIMARY KEY (`id`),
  KEY `revision` (`revision`),
  KEY `phone_id` (`phone_id`),
  KEY `runtime_id` (`runtime_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

--
-- Dumpning av Data i tabell `membench_testruns`
--

INSERT INTO `membench_testruns` (`id`, `timestamp`, `revision`, `runtime_id`, `git_hash`, `phone_id`, `native_sdk_ver`, `alloc_str_10`, `alloc_str_100`, `alloc_void_1`, `alloc_void_100`, `alloc_void_1000`, `alloc_dummy`, `alloc_dummy_struct`, `alloc_dummy_mix`, `access_array`, `access_vector`, `add_vector`, `access_dummy`, `access_dummy_struct`, `access_dummy_mix`) VALUES
(1, '0000-00-00 00:00:00', '1337', 1, '987123ab', 3, '2\n', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

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
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4 ;

--
-- Dumpning av Data i tabell `phones`
--

INSERT INTO `phones` (`phone_id`, `phone_name`) VALUES
(1, 'xperia'),
(3, 'HTC Wildfire');

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
