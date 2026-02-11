-- Deploy messaging:20260211103226_create_groups_member_table to pg

BEGIN;

CREATE TABLE IF NOT EXISTS group_members (
  group_id    INTEGER NOT NULL,
  user_id     INTEGER NOT NULL,
  role        TEXT NOT NULL DEFAULT 'member',
  joined_at   TIMESTAMPTZ NOT NULL DEFAULT NOW(),
  updated_at  TIMESTAMPTZ NOT NULL DEFAULT NOW(),

  PRIMARY KEY (group_id, user_id),

  CONSTRAINT group_members_group_fk
    FOREIGN KEY (group_id) REFERENCES groups(group_id)
    ON DELETE CASCADE
);

CREATE INDEX IF NOT EXISTS idx_group_members_group_id
  ON group_members(group_id);

CREATE INDEX IF NOT EXISTS idx_group_members_user_id
  ON group_members(user_id);

CREATE OR REPLACE FUNCTION set_updated_at()
RETURNS TRIGGER AS $$
BEGIN
  NEW.updated_at = NOW();
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trg_group_members_set_updated_at ON group_members;

CREATE TRIGGER trg_group_members_set_updated_at
BEFORE UPDATE ON group_members
FOR EACH ROW
EXECUTE FUNCTION set_updated_at();

COMMIT;
