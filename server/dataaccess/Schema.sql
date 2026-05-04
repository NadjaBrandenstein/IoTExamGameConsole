-- ============================
-- CREATE SCHEMA IF NOT EXISTS
-- ============================
CREATE SCHEMA IF NOT EXISTS GameConsole;

-- Make sure all following tables go into this schema
SET search_path TO GameConsole;

-- ============================
-- DROP TABLES
-- ============================
DROP TABLE IF EXISTS SimonSaysScores CASCADE;
DROP TABLE IF EXISTS WhackAMoleScores CASCADE;
DROP TABLE IF EXISTS RedLightGreenLightScores CASCADE;

-- ============================
-- SIMON SAYS TABLE
-- ============================
CREATE TABLE SimonSaysScores (
                                 id SERIAL PRIMARY KEY,
                                 player_name TEXT NOT NULL,
                                 score INT NOT NULL,
                                 created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP
);

-- ============================
-- WHACK-A-MOLE TABLE
-- ============================
CREATE TABLE WhackAMoleScores (
                                  id SERIAL PRIMARY KEY,
                                  player_name TEXT NOT NULL,
                                  score INT NOT NULL,
                                  created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP
);

-- ============================
-- RED LIGHT, GREEN LIGHT TABLE
-- ============================
CREATE TABLE RedLightGreenLightScores (
                                          id SERIAL PRIMARY KEY,
                                          player_name TEXT NOT NULL,
                                          score INT NOT NULL,
                                          created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP
);

-- ============================
-- INDEXES FOR FAST LEADERBOARDS
-- ============================
CREATE INDEX idx_simon_score ON SimonSaysScores(score DESC);
CREATE INDEX idx_whack_score ON WhackAMoleScores(score DESC);
CREATE INDEX idx_redlight_score ON RedLightGreenLightScores(score DESC);