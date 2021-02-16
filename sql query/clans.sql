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
-- Struktura tabeli dla tabeli `clans`
--

CREATE TABLE `clans` (
  `idclans` int(11) NOT NULL,
  `createDate` datetime DEFAULT current_timestamp(),
  `name` varchar(64) COLLATE utf8_polish_ci NOT NULL,
  `info` varchar(42) COLLATE utf8_polish_ci NOT NULL DEFAULT 'Brak Ogloszenia',
  `members` int(11) NOT NULL DEFAULT 0,
  `nugget` int(11) NOT NULL DEFAULT 0,
  `kills` int(11) NOT NULL DEFAULT 0,
  `level` int(11) NOT NULL DEFAULT 0,
  `points` int(11) NOT NULL DEFAULT 0,
  `health` int(11) NOT NULL DEFAULT 0,
  `expDrop` int(11) NOT NULL DEFAULT 0,
  `nuggetDrop` int(11) NOT NULL DEFAULT 0,
  `critic` int(11) NOT NULL DEFAULT 0,
  `damagePlayer` int(11) NOT NULL DEFAULT 0,
  `damageClass` int(11) NOT NULL DEFAULT 0,
  `cooldown` int(11) NOT NULL DEFAULT 0,
  `idmotd` varchar(20) COLLATE utf8_polish_ci NOT NULL DEFAULT '_',
  `upgradetime` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

--
-- Indeksy dla zrzutów tabel
--

--
-- Indeksy dla tabeli `clans`
--
ALTER TABLE `clans`
  ADD PRIMARY KEY (`idclans`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT dla tabeli `clans`
--
ALTER TABLE `clans`
  MODIFY `idclans` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
