-- Revert messaging:20260108112940_create_groups_and_messages from pg


BEGIN;

DROP TABLE IF EXISTS messages;
DROP TABLE IF EXISTS groups;

COMMIT;
