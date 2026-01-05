-- Verify auth:20251205093048_version_migrations on pg

BEGIN;

SELECT 1
FROM information_schema.columns
WHERE table_name = 'connexion'
  AND column_name = 'hashed_password';

SELECT 1
FROM information_schema.columns
WHERE table_name = 'connexion'
  AND column_name = 'salt';

ROLLBACK;