-- Verify messaging:20260211103226_create_groups_member_table on pg

BEGIN;

DO $$
BEGIN
  IF NOT EXISTS (
    SELECT 1
    FROM information_schema.tables
    WHERE table_schema = 'public'
      AND table_name = 'group_members'
  ) THEN
    RAISE EXCEPTION 'Table public.group_members is missing';
  END IF;

  PERFORM 1
  FROM information_schema.columns
  WHERE table_schema='public' AND table_name='group_members' AND column_name='group_id';
  IF NOT FOUND THEN RAISE EXCEPTION 'Column group_members.group_id is missing'; END IF;

  PERFORM 1
  FROM information_schema.columns
  WHERE table_schema='public' AND table_name='group_members' AND column_name='user_id';
  IF NOT FOUND THEN RAISE EXCEPTION 'Column group_members.user_id is missing'; END IF;

  PERFORM 1
  FROM information_schema.columns
  WHERE table_schema='public' AND table_name='group_members' AND column_name='role';
  IF NOT FOUND THEN RAISE EXCEPTION 'Column group_members.role is missing'; END IF;

  PERFORM 1
  FROM information_schema.columns
  WHERE table_schema='public' AND table_name='group_members' AND column_name='joined_at';
  IF NOT FOUND THEN RAISE EXCEPTION 'Column group_members.joined_at is missing'; END IF;

  PERFORM 1
  FROM information_schema.columns
  WHERE table_schema='public' AND table_name='group_members' AND column_name='updated_at';
  IF NOT FOUND THEN RAISE EXCEPTION 'Column group_members.updated_at is missing'; END IF;
END $$;

DO $$
BEGIN
  IF NOT EXISTS (
    SELECT 1
    FROM pg_constraint c
    JOIN pg_class r ON r.oid = c.conrelid
    JOIN pg_namespace n ON n.oid = r.relnamespace
    WHERE n.nspname = 'public'
      AND r.relname = 'group_members'
      AND c.contype = 'p'
      AND pg_get_constraintdef(c.oid) LIKE 'PRIMARY KEY (group_id, user_id)%'
  ) THEN
    RAISE EXCEPTION 'Primary key (group_id, user_id) is missing on group_members';
  END IF;
END $$;

DO $$
BEGIN
  IF NOT EXISTS (
    SELECT 1
    FROM pg_constraint c
    JOIN pg_class r ON r.oid = c.conrelid
    JOIN pg_namespace n ON n.oid = r.relnamespace
    WHERE n.nspname = 'public'
      AND r.relname = 'group_members'
      AND c.contype = 'f'
      AND pg_get_constraintdef(c.oid) LIKE 'FOREIGN KEY (group_id) REFERENCES groups(group_id) ON DELETE CASCADE%'
  ) THEN
    RAISE EXCEPTION 'Foreign key group_id -> groups(group_id) ON DELETE CASCADE is missing';
  END IF;
END $$;

DO $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM pg_indexes
    WHERE schemaname='public'
      AND tablename='group_members'
      AND indexname='idx_group_members_user_id'
  ) THEN
    RAISE EXCEPTION 'Index idx_group_members_user_id is missing';
  END IF;

  IF NOT EXISTS (
    SELECT 1 FROM pg_indexes
    WHERE schemaname='public'
      AND tablename='group_members'
      AND indexname='idx_group_members_group_id'
  ) THEN
    RAISE EXCEPTION 'Index idx_group_members_group_id is missing';
  END IF;
END $$;

DO $$
BEGIN
  IF NOT EXISTS (
    SELECT 1
    FROM pg_trigger t
    JOIN pg_class r ON r.oid = t.tgrelid
    JOIN pg_namespace n ON n.oid = r.relnamespace
    WHERE n.nspname='public'
      AND r.relname='group_members'
      AND t.tgname='trg_group_members_set_updated_at'
      AND NOT t.tgisinternal
  ) THEN
    RAISE EXCEPTION 'Trigger trg_group_members_set_updated_at is missing on group_members';
  END IF;
END $$;
ROLLBACK;
