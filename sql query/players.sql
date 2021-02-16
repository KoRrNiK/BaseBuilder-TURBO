-- phpMyAdmin SQL Dump
-- version 4.9.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Czas generowania: 16 Lut 2021, 12:00
-- Wersja serwera: 10.4.8-MariaDB
-- Wersja PHP: 7.1.32

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Baza danych: `testowabaza`
--

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `players`
--

CREATE TABLE `players` (
  `idplayer` int(11) NOT NULL,
  `name` varchar(33) COLLATE utf8_polish_ci NOT NULL,
  `pass` varchar(12) COLLATE utf8_polish_ci NOT NULL DEFAULT '_',
  `kills` int(11) NOT NULL DEFAULT 0,
  `deaths` int(11) NOT NULL DEFAULT 0,
  `points` int(11) NOT NULL DEFAULT 0,
  `timePlay` int(11) NOT NULL DEFAULT 0,
  `firstlogin` datetime DEFAULT current_timestamp(),
  `lastlogin` datetime DEFAULT current_timestamp(),
  `maxDamage` int(11) NOT NULL DEFAULT 0,
  `clan` int(11) NOT NULL DEFAULT 0,
  `flag` int(11) NOT NULL DEFAULT 0,
  `clannugget` int(11) NOT NULL DEFAULT 0,
  `ipLogin` varchar(32) COLLATE utf8_polish_ci NOT NULL DEFAULT '_',
  `sidLogin` varchar(35) COLLATE utf8_polish_ci NOT NULL DEFAULT '_',
  `nuggets` int(11) NOT NULL DEFAULT 45,
  `bones` int(11) NOT NULL DEFAULT 10,
  `level` int(11) NOT NULL DEFAULT 1,
  `xp` float NOT NULL DEFAULT 0,
  `reset` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

--
-- Indeksy dla zrzutów tabel
--

--
-- Indeksy dla tabeli `players`
--
ALTER TABLE `players`
  ADD PRIMARY KEY (`idplayer`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT dla tabeli `players`
--
ALTER TABLE `players`
  MODIFY `idplayer` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
