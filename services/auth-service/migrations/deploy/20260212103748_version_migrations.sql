-- Deploy auth:20260212103748_version_migrations to pg

-- Add a new column 'role' to 'users' table and create a new table 'account_status' to track user account statuses.
BEGIN;

ALTER TABLE users
ADD COLUMN role INT DEFAULT 0 NOT NULL;

CREATE TABLE roles (
    id SERIAL PRIMARY KEY,
    role_name VARCHAR(50) NOT NULL UNIQUE,
    role_description TEXT
);

INSERT INTO roles (role_name, role_description) VALUES
    ('user', 'Regular user with limited permissions'),
    ('admin', 'Administrator with full permissions');

COMMIT;
