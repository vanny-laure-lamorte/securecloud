-- Revert messaging:20260108113056_insert_test_messages from pg

BEGIN;

DELETE FROM messages
WHERE content LIKE 'PERS:%' OR content LIKE 'GRP1:%' OR content LIKE 'GRP2:%';

DELETE FROM groups WHERE name IN ('group1', 'group2');

COMMIT;
