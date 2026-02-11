-- Verify messaging:20260108112940_create_groups_and_messages on pg

BEGIN;

SELECT 1
FROM information_schema.tables
WHERE table_name IN ('groups','messages');

SELECT 1
FROM pg_constraint
WHERE conname = 'exactly_one_receiver_chk';

ROLLBACK;