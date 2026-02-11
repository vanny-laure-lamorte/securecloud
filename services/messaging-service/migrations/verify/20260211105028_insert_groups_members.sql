-- Verify messaging:20260211105028_insert_groups_members on pg

BEGIN;

DO $$
DECLARE
  expected_count INTEGER := 6;
  actual_count   INTEGER;
BEGIN
  SELECT COUNT(*) INTO actual_count
  FROM group_members
  WHERE (group_id, user_id) IN (
    (1,1),
    (1,2),
    (1,3),
    (2,1),
    (2,2),
    (2,3)
  );

  IF actual_count <> expected_count THEN
    RAISE EXCEPTION
      'Seed verification failed: expected %, found %',
      expected_count, actual_count;
  END IF;
END $$;

ROLLBACK;