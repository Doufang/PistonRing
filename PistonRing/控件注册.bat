@echo off
@echo 注册当前目录下的dll和ocx文件
pause

color a

for /r %%i in (*.ocx) do (echo 正在注冊 %%~nxi...請稍後... 目录 %%i )

echo 注冊完成,按任意鍵退出...
pause>nul
exit