-- Revert messaging:20260211105028_insert_groups_members from pg

BEGIN;

DELETE FROM group_members
WHERE (group_id, user_id) IN (
  (1,1),
  (1,2),
  (1,3),
  (2,1),
  (2,2),
  (2,3)
);

COMMIT;