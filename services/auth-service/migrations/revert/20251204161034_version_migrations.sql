-- Revert auth:20251204161034_version_migrations from pg

BEGIN;

DROP TABLE IF EXISTS users CASCADE;

COMMIT;