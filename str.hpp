#ifndef STR_HPP
#define STR_HPP

#include <algorithm>
#include <cstring>
#include <vector>
#include <cstddef>

class str
{
private:
	char *buf_;
	size_t len_;

	void init_empty() {
		buf_ = new char[1];
		buf_[0] = '\0';
		len_ = 0;
	}

	void assign_cstr(const char *s) {
		if (s == nullptr) {
			init_empty();
			return;
		}
		len_ = std::strlen(s);
		buf_ = new char[len_ + 1];
		std::memcpy(buf_, s, len_ + 1);
	}

public:
	str() { init_empty(); }
	str(const char &c) {
		buf_ = new char[2];
		buf_[0] = c;
		buf_[1] = '\0';
		len_ = 1;
	}

	str(const char *&& s_) { assign_cstr(s_); }

	str(const char *s_) { assign_cstr(s_); }
	str(char *s_) { assign_cstr(s_); }

	str &operator=(const char *s_) {
		delete[] buf_;
		assign_cstr(s_);
		return *this;
	}
	str &operator=(char *s_) {
		delete[] buf_;
		assign_cstr(s_);
		return *this;
	}


	str &operator=(const char *&& s_) {
		if (buf_ != nullptr) {
			delete[] buf_;
		}
		assign_cstr(s_);
		return *this;
	}

	str(const str &other) {
		len_ = other.len_;
		buf_ = new char[len_ + 1];
		std::memcpy(buf_, other.buf_, len_ + 1);
	}

	str &operator=(const str &other) {
		if (this == &other) {
			return *this;
		}
		char *new_buf = new char[other.len_ + 1];
		std::memcpy(new_buf, other.buf_, other.len_ + 1);
		delete[] buf_;
		buf_ = new_buf;
		len_ = other.len_;
		return *this;
	}

	char &operator[](size_t pos) { return buf_[pos]; }
	const char &operator[](size_t pos) const { return buf_[pos]; }


	size_t len() const { return len_; }

	str join(const std::vector<str> &strs) const {
		if (strs.empty()) {
			return str();
		}

		size_t total = 0;
		for (const auto &s : strs) {
			total += s.len_;
		}
		total += len_ * (strs.size() - 1);

		str result;
		delete[] result.buf_;
		result.buf_ = new char[total + 1];
		result.len_ = total;

		char *p = result.buf_;
		for (size_t i = 0; i < strs.size(); ++i) {
			if (i != 0 && len_ > 0) {
				std::memcpy(p, buf_, len_);
				p += len_;
			}
			if (i != 0 && len_ == 0) {
				// nothing to copy for an empty separator
			}
			std::memcpy(p, strs[i].buf_, strs[i].len_);
			p += strs[i].len_;
		}
		result.buf_[total] = '\0';
		return result;
	}

	str slice(size_t l, size_t r) const {
		if (l > r) {
			return str();
		}
		l = std::min(l, len_);
		r = std::min(r, len_);
		if (l >= r) {
			return str();
		}
		str result;
		delete[] result.buf_;
		result.len_ = r - l;
		result.buf_ = new char[result.len_ + 1];
		std::memcpy(result.buf_, buf_ + l, result.len_);
		result.buf_[result.len_] = '\0';
		return result;
	}

	~str() { delete[] buf_; }
};

#endif
