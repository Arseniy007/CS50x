-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Check the crime_scene on the right date
SELECT description FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28
AND street = 'Humphrey Street';

-- Check the above-mentioned interviews
SELECT transcript FROM interviews
WHERE year = 2021
AND month = 7
AND  day = 28;

-- Check the security footage from the bakery parking lot
SELECT license_plate, activity FROM bakery_security_logs
WHERE year = 2021
AND month = 7
AND day = 28
AND hour = 10
AND minute > 4 A
AND minute < 26;

-- Check the atm logs on the Leggett Street
SELECT name FROM people
JOIN bank_accounts
ON people.id = bank_accounts.person_id
JOIN atm_transactions
ON bank_accounts.account_number = atm_transactions.account_number
WHERE year = 2021
AND month = 7
AND day = 28
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw';

-- Check the phonecall logs based on the information from the interview
SELECT caller, receiver FROM phone_calls
WHERE year = 2021
AND month = 7
AND day = 28
AND duration <= 60;

-- Merge all the information together
SELECT name, passport_number FROM people
JOIN bakery_security_logs
ON people.license_plate = bakery_security_logs.license_plate
JOIN phone_calls
ON people.phone_number = phone_calls.caller
WHERE bakery_security_logs.year = 2021
AND bakery_security_logs.month = 7
AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute > 4
AND bakery_security_logs.minute < 26
AND phone_calls.year = 2021
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration <= 60;

-- Get the name of the airport in
SELECT abbreviation, full_name FROM airports
WHERE city = 'Fiftyville';

-- Try to get more narrow circle and the names of airports
SELECT people.name, airports.full_name FROM people
JOIN bakery_security_logs
ON people.license_plate = bakery_security_logs.license_plate
JOIN phone_calls
ON people.phone_number = phone_calls.caller
JOIN passengers
ON people.passport_number = passengers.passport_number
JOIN flights
ON passengers.flight_id = flights.id
JOIN airports
ON flights.destination_airport_id = airports.id
WHERE bakery_security_logs.year = 2021
AND bakery_security_logs.month = 7
AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute > 4
AND bakery_security_logs.minute < 26
AND phone_calls.year = 2021
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration <= 60
AND bakery_security_logs.activity = 'exit'
AND flights.day = 29;

-- Get the earliest flight of the day and narrowing the names of suspects
SELECT people.name, airports.full_name, flights.hour, flights.minute FROM people
JOIN bakery_security_logs
ON people.license_plate = bakery_security_logs.license_plate
JOIN phone_calls
ON people.phone_number = phone_calls.caller
JOIN passengers
ON people.passport_number = passengers.passport_number
JOIN flights
ON passengers.flight_id = flights.id
JOIN airports
ON flights.destination_airport_id = airports.id
WHERE bakery_security_logs.year = 2021
AND bakery_security_logs.month = 7
AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute >= 15
AND bakery_security_logs.minute <= 25
AND phone_calls.year = 2021
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration <= 60
AND bakery_security_logs.activity = 'exit'
AND flights.day = 29
AND atm_transactions.year = 2021
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_transactions.atm_location = 'Leggett Street'
AND atm_transactions.transaction_type = 'withdraw'
ORDER BY flights.hour, flights.minute;

-- Putting all the information together
SELECT people.name, airports.full_name, flights.hour, flights.minute FROM people
JOIN bakery_security_logs
ON people.license_plate = bakery_security_logs.license_plate
JOIN phone_calls
ON people.phone_number = phone_calls.caller
JOIN passengers
ON people.passport_number = passengers.passport_number
JOIN flights
ON passengers.flight_id = flights.id
JOIN airports
ON flights.destination_airport_id = airports.id
JOIN bank_accounts
ON people.id = bank_accounts.person_id
JOIN atm_transactions
ON bank_accounts.account_number = atm_transactions.account_number
WHERE bakery_security_logs.year = 2021
AND bakery_security_logs.month = 7
AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute >= 15
AND bakery_security_logs.minute <= 25
AND phone_calls.year = 2021
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration <= 60
AND bakery_security_logs.activity = 'exit'
AND flights.day = 29
AND atm_transactions.year = 2021
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_transactions.atm_location = 'Leggett Street'
AND atm_transactions.transaction_type = 'withdraw'
ORDER BY flights.hour, flights.minute;

-- The thief must be Bruce!
-- Get the ingormation about the escape city
SELECT city FROM airports
WHERE full_name = 'LaGuardia Airport';

-- Get the number of accomplice
SELECT receiver FROM phone_calls
WHERE year = 2021
AND month = 7
AND day = 28
AND duration <= 60
AND caller IN
(SELECT phone_number FROM people
WHERE name = 'Bruce');

-- Get his name
SELECT name FROM people
WHERE phone_number = '(375) 555-8161';