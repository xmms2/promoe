/*-
 * Copyright (C) 2010 Thomas Frauendorfer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// On Debian, libarchive is compiled with 64bit off_t, but nothing in the
// libarchive headers tells so. In other words: Debian sucks.
#define _LARGEFILE_SOURCE 1
#define _FILE_OFFSET_BITS 64

#include <archive.h>
#include <QIODevice>

#include <QtDebug>

#if (ARCHIVE_API_VERSION < 2)
typedef __LA_SSIZE_T archive_skip_t;
#elif (ARCHIVE_API_VERSION < 3)
typedef off_t archive_skip_t;
#else
typedef __LA_INT64_T archive_skip_t;
#endif

static const int READ_MAX = 4096;

struct read_qiodevice_data {
	QIODevice *device;
	char buffer[READ_MAX];
};

static int qiodevice_read_close(struct archive *, void *);
static archive_skip_t qiodevice_read_skip(struct archive *, void *, archive_skip_t);
static __LA_SSIZE_T qiodevice_read(struct archive *, void *, const void **);

int
archive_read_open_qiodevice(struct archive *a, QIODevice *device)
{
	if (device == 0) {
		qDebug () << "archive_read_open_qiodevice";
		archive_set_error(a, -1, "Invalid QIODevice");
		return ARCHIVE_FATAL;
	}

	if (!device->isOpen() && !device->open(QIODevice::ReadOnly)) {
		qDebug () << "archive_read_open_qiodevice";
		archive_set_error(a, -1, "Could not open QIODevice for reading");
		return ARCHIVE_FATAL;
	}

	if (!device->isReadable()) {
		qDebug () << "archive_read_open_qiodevice";
		archive_set_error(a, -1, "Could not open QIODevice for reading");
		return ARCHIVE_FATAL;
	}

	struct read_qiodevice_data *data = new read_qiodevice_data;
	if (data == 0) {
		qDebug () << "archive_read_open_qiodevice";
		archive_set_error(a, -1, "No Memory");
		return ARCHIVE_FATAL;
	}
	data->device = device;

	device->reset(); // Seek to begining of file
	device->setTextModeEnabled(false); // Binary mode

	return (archive_read_open2(a, data, NULL, qiodevice_read,
	        qiodevice_read_skip, qiodevice_read_close));
}

static int
qiodevice_read_close(struct archive *a, void *v)
{
	const struct read_qiodevice_data *data = static_cast<read_qiodevice_data *>(v);

	(void)a;
	if (data != 0) {
		delete data;
	}

	return ARCHIVE_OK;
}

static archive_skip_t
qiodevice_read_skip(struct archive *a, void *v, archive_skip_t _request)
{
	const struct read_qiodevice_data *data = static_cast<read_qiodevice_data *>(v);

	(void)a; /* unused */
	const qint64 request = _request; // make sure we work with 64bit values


	if (data->device->isSequential()) {
		return 0;
	}
	if (request == 0) {
		return 0;
	}

	const qint64 oldpos = data->device->pos();
	if (data->device->seek(oldpos + request)) {
		return request;
	}

	// Seek might have failed because we tried to seek beyond the end of a file
	qDebug("Seek failed, seeked: %lli", data->device->pos() - oldpos);

	return data->device->pos() - oldpos;
}

static __LA_SSIZE_T
qiodevice_read(struct archive *a, void *v, const void **buf)
{
	struct read_qiodevice_data *data = static_cast<read_qiodevice_data *>(v);

	(void)a;

	*buf = data->buffer;
	qint64 bytes_read = data->device->read(data->buffer, READ_MAX);

	if (bytes_read < 0) {
		archive_set_error(a, 1, "Error reading from QIODevice");
	}

	return bytes_read;
}


