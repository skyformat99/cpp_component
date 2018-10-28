import os
import re

class CWriteCmake(object):
	# 找到指定根目录下的所有含有 CMakeLists.txt 文件的目录, 并写入 .cmake 文件
	def __init__(self, root):
		self.m_root = root

	def __filter_filepath(self, path):
		return re.sub(r"\\", "/", path)

	def clear_write(self, content, path):
		fp = None
		try:
			fp = open(path, "w", encoding="utf8")
			fp.write(content)
		except Exception as e:
			raise RuntimeError("path error")
		else:
			pass
		finally:
			if fp is not None:
				fp.close()

	def write(self, path):
		content = ""
		for dirname, dirs, files in os.walk(self.m_root):
			dirname = self.__filter_filepath(dirname)
			if "CMakeLists.txt" in files:
				content += "add_subdirectory(${PROJECT_SOURCE_DIR}/script/" + dirname + ")\n"
		self.clear_write(content, path)


if __name__ == "__main__":
	finder = CWriteCmake("../component")
	finder.write("./makelists.cmake")
