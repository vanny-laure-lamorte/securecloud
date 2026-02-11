-- Deploy messaging:20260211105028_insert_groups_members to pg

BEGIN;

INSERT INTO group_members (group_id, user_id, role)
VALUES
  (1, 1, 'admin'),
  (1, 2, 'member'),
  (1, 3, 'member'),
  (2, 1, 'member'),
  (2, 2, 'admin'),
  (2, 3, 'member')
ON CONFLICT (group_id, user_id) DO NOTHING;
COMMIT;
