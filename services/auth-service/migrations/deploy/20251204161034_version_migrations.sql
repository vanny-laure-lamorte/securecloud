-- Deploy auth:20251204161034_version_migrations to pg

BEGIN;

CREATE TABLE IF NOT EXISTS users (
    user_id      SERIAL PRIMARY KEY,
    username     TEXT NOT NULL,
    first_name   TEXT,
    last_name    TEXT,
    created_at   TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at   TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

COMMIT;