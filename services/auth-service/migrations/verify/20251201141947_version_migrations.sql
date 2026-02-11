-- Verify auth:20251201141947_version_migrations on pg

BEGIN;

SELECT 1 FROM connexion LIMIT 1;

ROLLBACK;