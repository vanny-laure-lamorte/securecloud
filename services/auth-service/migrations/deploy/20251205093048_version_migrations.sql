-- Deploy auth:20251205093048_version_migrations to pg

BEGIN;

ALTER TABLE connexion
    RENAME COLUMN password TO hashed_password;

ALTER TABLE connexion
    ADD COLUMN IF NOT EXISTS salt VARCHAR(255);

COMMIT;