package bitcask

import "errors"

var (
	ErrKeyNotFound         = errors.New("key not found in database")
	ErrInvalidDatabaseFile = errors.New("invalid database file")
)
