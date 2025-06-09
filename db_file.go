package bitcask

import (
	"os"
	"path/filepath"
	"sync"
)

const FileName = "bitcask.data"
const MergeFileName = "bitcask.merge"

type DatabaseFile struct {
	File          *os.File
	Offset        int64
	HeaderBufPool *sync.Pool
}

func newInternal(fileName string) (*DatabaseFile, error) {
	file, err := os.OpenFile(fileName, os.O_CREATE|os.O_RDWR, 0644)
	if err != nil {
		return nil, err
	}
	stat, err := os.Stat(fileName)
	if err != nil {
		return nil, err
	}
	pool := &sync.Pool{New: func() interface{} {
		return make([]byte, entryHeaderSize)
	}}
	return &DatabaseFile{
		Offset:        stat.Size(),
		File:          file,
		HeaderBufPool: pool,
	}, nil
}

func NewDataBaseFile(path string) (*DatabaseFile, error) {
	fileName := filepath.Join(path, FileName)
	return newInternal(fileName)
}

func NewMergeDatabaseFile(path string) (*DatabaseFile, error) {
	fileName := filepath.Join(path, MergeFileName)
	return newInternal(fileName)
}

func (df *DatabaseFile) Read(offset int64) (e *Entry, err error) {
	buf := df.HeaderBufPool.Get().([]byte)
	defer df.HeaderBufPool.Put(buf)
	if _, err = df.File.ReadAt(buf, offset); err != nil {
		return
	}
	if e, err = Decode(buf); err != nil {
		return
	}
	offset += entryHeaderSize
	if e.KeySize > 0 {
		key := make([]byte, e.KeySize)
		if _, err = df.File.ReadAt(key, offset); err != nil {
			return
		}
		e.Key = key
	}

	offset += int64(e.KeySize)
	if e.ValueSize > 0 {
		value := make([]byte, e.ValueSize)
		if _, err = df.File.ReadAt(value, offset); err != nil {
			return
		}
		e.Value = value
	}
	return
}

func (df *DatabaseFile) Write(e *Entry) (err error) {
	enc, err := e.Encode()
	if err != nil {
		return err
	}
	_, err = df.File.WriteAt(enc, df.Offset)
	df.Offset += e.GetSize()
	return nil
}
