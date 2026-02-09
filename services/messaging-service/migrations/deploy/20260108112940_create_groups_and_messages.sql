-- Deploy messaging:20260108112940_create_groups_and_messages to pg

BEGIN;

CREATE TABLE IF NOT EXISTS groups (
  group_id    SERIAL PRIMARY KEY,
  name        TEXT NOT NULL UNIQUE,
  created_at  TIMESTAMPTZ NOT NULL DEFAULT NOW(),
  updated_at  TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE TABLE IF NOT EXISTS messages (
  message_id        SERIAL PRIMARY KEY,
  sender_id         INTEGER NOT NULL,
  receiver_user_id  INTEGER NULL,
  receiver_group_id INTEGER NULL,
  content           TEXT NOT NULL,
  created_at        TIMESTAMPTZ NOT NULL DEFAULT NOW(),
  updated_at        TIMESTAMPTZ NOT NULL DEFAULT NOW(),

  CONSTRAINT exactly_one_receiver_chk CHECK (
    (receiver_user_id IS NOT NULL AND receiver_group_id IS NULL)
 OR (receiver_user_id IS NULL AND receiver_group_id IS NOT NULL)
  )
);

COMMIT;
