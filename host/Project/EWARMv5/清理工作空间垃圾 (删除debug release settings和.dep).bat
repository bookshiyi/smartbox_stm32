@rem ɾ���������ļ����ļ���

@rem for /r . %%a in (.) do @if exist "%%a\Debug" @echo "%%a\Debug"
@for /r . %%a in (.) do @if exist "%%a\Debug" rd /s /q "%%a\Debug"

@rem for /r . %%a in (.) do @if exist "%%a\Release" @echo "%%a\Release"
@for /r . %%a in (.) do @if exist "%%a\Release" rd /s /q "%%a\Release"

@rem for /r . %%a in (.) do @if exist "%%a\settings" @echo "%%a\settings"
@for /r . %%a in (.) do @if exist "%%a\settings" rd /s /q "%%a\settings"

@del /F /S /Q *.dep

@echo .
@echo     ȫ��������ɣ�ллʹ��     Rev2.0
@echo .
@echo                                        -----By Blade 
@pause