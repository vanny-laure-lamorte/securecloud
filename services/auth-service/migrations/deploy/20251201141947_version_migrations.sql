-- Deploy auth:20251201141947_version_migrations to pg

BEGIN;

-- Initial creation of connexion table for user authentication
CREATE TABLE connexion(
    id SERIAL NOT NULL,
    email varchar(255) NOT NULL,
    password varchar(255) NOT NULL,
    account_status varchar(50) DEFAULT 'active'::character varying,
    login_time timestamp without time zone DEFAULT CURRENT_TIMESTAMP,
    logout_time timestamp without time zone,
    user_id integer NOT NULL,
    PRIMARY KEY(id)
);

COMMIT;