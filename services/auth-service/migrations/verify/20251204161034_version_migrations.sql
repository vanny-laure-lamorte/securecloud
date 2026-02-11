-- Verify auth:20251204161034_version_migrations on pg

BEGIN;

SELECT 1 FROM users LIMIT 1;

ROLLBACK;