-- Revert messaging:20260211103226_create_groups_member_table from pg

BEGIN;

DROP TRIGGER IF EXISTS trg_group_members_set_updated_at ON group_members;

DO $$
BEGIN
  IF NOT EXISTS (
    SELECT 1
    FROM pg_trigger t
    JOIN pg_proc p ON p.oid = t.tgfoid
    WHERE p.proname = 'set_updated_at'
      AND NOT t.tgisinternal
  ) THEN
    DROP FUNCTION IF EXISTS set_updated_at();
  END IF;
END $$;

DROP INDEX IF EXISTS idx_group_members_user_id;
DROP INDEX IF EXISTS idx_group_members_group_id;

DROP TABLE IF EXISTS group_members;
COMMIT;
