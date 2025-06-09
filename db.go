package bitcask

import (
	"io"
	"os"
	"path/filepath"
	"sync"
)

type Bitcask struct {
	dbFile  *DatabaseFile
	indexes map[string]int64 // index in memory
	dirPath string
	mutex   sync.RWMutex
}

func Open(dirPath string) (*Bitcask, error) {
	if _, err := os.Stat(dirPath); os.IsNotExist(err) {
		if err := os.MkdirAll(dirPath, os.ModePerm); err != nil {
			return nil, err
		}
	}
	dirAbsPath, err := filepath.Abs(dirPath)
	if err != nil {
		return nil, err
	}
	dbFile, err := NewDataBaseFile(dirAbsPath)
	if err != nil {
		return nil, err
	}
	db := &Bitcask{
		dbFile:  dbFile,
		indexes: make(map[string]int64),
		dirPath: dirAbsPath,
	}
	db.loadIndexerFromFile()
	return db, nil
}

func (db *Bitcask) Put(key []byte, value []byte) (err error) {
	if len(key) == 0 {
		return
	}
	db.mutex.Lock()
	defer db.mutex.Unlock()

	offset := db.dbFile.Offset
	entry := newEntry(key, value, PUT)
	err = db.dbFile.Write(entry)
	db.indexes[string(key)] = offset
	return
}

func (db *Bitcask) exist(key []byte) (int64, error) {
	offset, ok := db.indexes[string(key)]
	if !ok {
		return 0, ErrKeyNotFound
	}
	return offset, nil
}

func (db *Bitcask) Get(key []byte) (val []byte, err error) {
	if len(key) == 0 {
		return
	}
	db.mutex.RLock()
	defer db.mutex.RUnlock()

	offset, err := db.exist(key)
	if err == ErrKeyNotFound {
		return
	}

	var e *Entry
	e, err = db.dbFile.Read(offset)
	if err != nil && err != io.EOF {
		return
	}
	if e != nil {
		val = e.Value
	}
	return
}

func (db *Bitcask) Del(key []byte) (err error) {
	if len(key) == 0 {
		return
	}

	db.mutex.Lock()
	defer db.mutex.Unlock()

	_, err = db.exist(key)
	if err == ErrKeyNotFound {
		err = nil
		return
	}

	e := newEntry(key, nil, DEL)
	err = db.dbFile.Write(e)
	if err != nil {
		return
	}

	delete(db.indexes, string(key))
	return
}

func (db *Bitcask) Merge() error {
	if db.dbFile.Offset == 0 {
		return nil
	}

	var (
		validEntries []*Entry
		offset       int64
	)
	for {
		e, err := db.dbFile.Read(offset)
		if err != nil {
			if err == io.EOF {
				break
			}
			return err
		}
		// index in memory are latest
		if off, ok := db.indexes[string(e.Key)]; ok && off == offset {
			validEntries = append(validEntries, e)
		}
		offset += e.GetSize()
	}
	// new file
	mergeFile, err := NewMergeDatabaseFile(db.dirPath)
	if err != nil {
		return err
	}
	defer func() {
		_ = os.Remove(mergeFile.File.Name())
	}()

	db.mutex.Lock()
	defer db.mutex.Unlock()

	for _, entry := range validEntries {
		writeOff := mergeFile.Offset
		err = mergeFile.Write(entry)
		if err != nil {
			return err
		}
		db.indexes[string(entry.Key)] = writeOff
	}

	dbFileName := db.dbFile.File.Name()
	_ = db.dbFile.File.Close()
	_ = os.Remove(dbFileName)
	_ = mergeFile.File.Close()
	mergeFileName := mergeFile.File.Name()
	_ = os.Rename(mergeFileName, filepath.Join(db.dirPath, FileName))

	dbFile, err := NewDataBaseFile(db.dirPath)
	if err != nil {
		return err
	}
	db.dbFile = dbFile
	return nil
}

func (db *Bitcask) loadIndexerFromFile() {
	if db.dbFile == nil {
		return
	}

	var offset int64
	for {
		e, err := db.dbFile.Read(offset)
		if err != nil {
			if err == io.EOF {
				break
			}
			return
		}

		db.indexes[string(e.Key)] = offset
		if e.Mark == DEL {
			delete(db.indexes, string(e.Key))
		}
		offset += e.GetSize()
	}
}

func (db *Bitcask) Close() error {
	if db.dbFile == nil {
		return ErrInvalidDatabaseFile
	}
	return db.dbFile.File.Close()
}
