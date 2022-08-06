-- Keep a log of any SQL queries you execute as you solve the mystery.
SELECT * FROM crime_scene_reports WHERE street = "Humphrey Street" AND year = 2021 AND month = 7 AND day = 28;
--crime_scene_reports id 295 time 10:15am
-- Interviews with bakery mentions at the same day
SELECT * FROM interviews WHERE day = 28 AND month = 7 AND year = 2021 AND transcript LIKE "%bakery%";
-- Interviewers names are Ruth, Eugene, Raymond
--  1- Ruth: thief has car and used bakery parking lot (check security logs) (10:15-10:25)
--  2- Eugene: (Bakery owner is Emma) Thief used an ATM in Leggett Street for withdrawing money earlier the day.
--  3- Raymond: Thief called someone(named accomplice as x) (call length < 1 min). Person x bought a flight ticket out of fiftyville (date 28/7/21 to later date)
-- 1- Checking info about first interview(Ruth) about bakery parking lot
SELECT * FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit";
--  within 10 minutes
SELECT * FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25;
-- Plates are: 5P2BI95 | 94KL13X       || 6P58WS2       || 4328GD8       || G412CB7       || L93JTIZ       || 322W7JE       || 0NTHK55
-- people names with above car plates
SELECT * FROM people WHERE license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25);
-- 2- Now checking for second interview (Eugene) for ATM
SELECT * FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";
--  checking account numbers in bank_accounts
SELECT * FROM bank_accounts WHERE bank_accounts.account_number IN (SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw");
-- matching people.id with bank_accounts.person_id
SELECT * FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN (SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"));
-- Combining both car data and bank data to narrow
SELECT * FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN
(SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND people.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25);
-- narrowed it to 4 people Bruce, Diana, Iman, Luca
-- 3- Now checking for third interview(Raymond)
-- 3a- Thief used phone
SELECT * FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60
-- Combining car,bank and phone data to narrow
SELECT * FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN
(SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND people.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25)
AND people.phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60);
-- narrowed it to 2 people Bruce, Diana
-- 3b- x bought a ticket for thief to out of fiftyville
-- Checking for flights of Bruce and Diana
SELECT * FROM passengers
WHERE passport_number IN (SELECT people.passport_number FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN
(SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND people.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25)
AND people.phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60));
-- 4 Flights found, Checking origin location as Fiftyville and day 29 and ordered by hour
SELECT * FROM flights
WHERE id IN (SELECT flight_id FROM passengers
WHERE passport_number IN (SELECT people.passport_number FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN
(SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND people.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25)
AND people.phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60)))
AND origin_airport_id IN (SELECT id FROM airports WHERE city = "Fiftyville" AND day = 29) ORDER BY flights.hour;
--2 Flights found, earliest flight is 8:20
--Flight id is 36
-- Check passanger for that flight
SELECT * FROM passengers WHERE
flight_id IN (SELECT id FROM flights
WHERE id IN (SELECT flight_id FROM passengers
WHERE passport_number IN (SELECT people.passport_number FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN
(SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND people.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25)
AND people.phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60)))
AND origin_airport_id IN (SELECT id FROM airports WHERE city = "Fiftyville" AND day = 29) ORDER BY flights.hour LIMIT 1);
-- 8 Passenger found, match passport number with bruce and diana
SELECT * FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN
(SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND people.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25)
AND people.phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60)
AND people.passport_number IN
(SELECT passport_number FROM passengers WHERE
flight_id IN (SELECT id FROM flights
WHERE id IN (SELECT flight_id FROM passengers
WHERE passport_number IN (SELECT people.passport_number FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN
(SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND people.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25)
AND people.phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60)))
AND origin_airport_id IN (SELECT id FROM airports WHERE city = "Fiftyville" AND day = 29) ORDER BY flights.hour LIMIT 1));
-- Thief is Bruce
--Bruce's phone call reciever's phone number(x's number)
SELECT phone_calls.receiver FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60 AND
caller IN (SELECT phone_number FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN
(SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND people.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25)
AND people.phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60)
AND people.passport_number IN
(SELECT passport_number FROM passengers WHERE
flight_id IN (SELECT id FROM flights
WHERE id IN (SELECT flight_id FROM passengers
WHERE passport_number IN (SELECT people.passport_number FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN
(SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND people.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25)
AND people.phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60)))
AND origin_airport_id IN (SELECT id FROM airports WHERE city = "Fiftyville" AND day = 29) ORDER BY flights.hour LIMIT 1)));
--Name of x
SELECT * FROM people
WHERE people.phone_number IN (SELECT phone_calls.receiver FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60 AND
caller IN (SELECT phone_number FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN
(SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND people.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25)
AND people.phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60)
AND people.passport_number IN
(SELECT passport_number FROM passengers WHERE
flight_id IN (SELECT id FROM flights
WHERE id IN (SELECT flight_id FROM passengers
WHERE passport_number IN (SELECT people.passport_number FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN
(SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND people.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25)
AND people.phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60)))
AND origin_airport_id IN (SELECT id FROM airports WHERE city = "Fiftyville" AND day = 29) ORDER BY flights.hour LIMIT 1))));
-- x is Robin
-- Bruce's destination with flight is
SELECT * FROM airports
WHERE id IN
(SELECT destination_airport_id FROM flights
WHERE id IN (SELECT flight_id FROM passengers
WHERE passport_number IN (SELECT people.passport_number FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.id IN
(SELECT bank_accounts.person_id FROM bank_accounts WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND people.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25)
AND people.phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60)))
AND origin_airport_id IN (SELECT id FROM airports WHERE city = "Fiftyville" AND day = 29) ORDER BY flights.hour LIMIT 1);
-- New York City
-- I always trusted to Bruce and Robin