-- Verify messaging:20260108113056_insert_test_messages on pg

BEGIN;

SELECT COUNT(*) >= 2 AS ok FROM groups WHERE name IN ('group1','group2');

SELECT COUNT(*) >= 6 AS ok FROM messages;

ROLLBACK;