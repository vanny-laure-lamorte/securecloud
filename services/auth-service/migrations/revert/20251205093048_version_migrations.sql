-- Revert auth:20251205093048_version_migrations from pg

BEGIN;

-- Supprimer la colonne salt
ALTER TABLE connexion
    DROP COLUMN IF EXISTS salt;

-- Renommer hashed_password -> password
ALTER TABLE connexion
    RENAME COLUMN hashed_password TO password;

COMMIT;