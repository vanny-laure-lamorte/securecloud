-- Deploy messaging:20260108113056_insert_test_messages to pg

BEGIN;

INSERT INTO groups (name) VALUES ('group1') ON CONFLICT (name) DO NOTHING;
INSERT INTO groups (name) VALUES ('group2') ON CONFLICT (name) DO NOTHING;

WITH g AS (
  SELECT
    (SELECT group_id FROM groups WHERE name='group1') AS g1,
    (SELECT group_id FROM groups WHERE name='group2') AS g2
)
INSERT INTO messages (sender_id, receiver_user_id, receiver_group_id, content, created_at, updated_at)
VALUES
-- 1 -> 2 personal
(1, 2, NULL, 'PERS: user1 -> user2', NOW() - INTERVAL '6 minutes', NOW()),
-- 2 -> 1 personal
(2, 1, NULL, 'PERS: user2 -> user1', NOW() - INTERVAL '5 minutes', NOW()),
-- 1 -> group1
(1, NULL, (SELECT g1 FROM g), 'GRP1: user1 -> group1', NOW() - INTERVAL '4 minutes', NOW()),
-- 2 -> group1
(2, NULL, (SELECT g1 FROM g), 'GRP1: user2 -> group1', NOW() - INTERVAL '3 minutes', NOW()),
-- 1 -> group2
(1, NULL, (SELECT g2 FROM g), 'GRP2: user1 -> group2', NOW() - INTERVAL '2 minutes', NOW()),
-- 2 -> group2
(2, NULL, (SELECT g2 FROM g), 'GRP2: user2 -> group2', NOW() - INTERVAL '1 minutes', NOW());

COMMIT;