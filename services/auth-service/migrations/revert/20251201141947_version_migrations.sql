-- Revert auth:20251201141947_version_migrations from pg

BEGIN;

DROP TABLE IF EXISTS connexion;

COMMIT;