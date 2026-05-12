-- ============================
-- CREATE SCHEMA IF NOT EXISTS
-- ============================
CREATE SCHEMA IF NOT EXISTS GameConsole;

-- Make sure all following tables go into this schema
SET search_path TO GameConsole;

-- ============================
-- DROP TABLES
-- ============================
DROP TABLE IF EXISTS BirdieSaysScores CASCADE;
DROP TABLE IF EXISTS WhackABirdScores CASCADE;
DROP TABLE IF EXISTS RedBirdGreenBirdScores CASCADE;

-- ============================
-- SIMON SAYS TABLE
-- ============================
CREATE TABLE BirdieSaysScores (
                                 id SERIAL PRIMARY KEY,
                                 player_name TEXT NOT NULL,
                                 score INT NOT NULL,
                                 created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP
);

-- ============================
-- WHACK-A-MOLE TABLE
-- ============================
CREATE TABLE WhackABirdScores (
                                  id SERIAL PRIMARY KEY,
                                  player_name TEXT NOT NULL,
                                  score INT NOT NULL,
                                  created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP
);

-- ============================
-- RED LIGHT, GREEN LIGHT TABLE
-- ============================
CREATE TABLE RedBirdGreenBirdScores (
                                          id SERIAL PRIMARY KEY,
                                          player_name TEXT NOT NULL,
                                          score INT NOT NULL,
                                          created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP
);

-- ============================
-- INDEXES FOR FAST LEADERBOARDS
-- ============================
CREATE INDEX idx_simon_score ON BirdieSaysScores(score DESC);
CREATE INDEX idx_whack_score ON WhackABirdScores(score DESC);
CREATE INDEX idx_redlight_score ON RedBirdGreenBirdScores(score DESC);