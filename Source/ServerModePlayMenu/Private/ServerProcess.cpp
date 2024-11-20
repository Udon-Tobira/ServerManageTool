// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerProcess.h"

ServerProcess::ServerProcess(const FString& MapName, int32 Port) {
	// UnrealEditor.exe �̃p�X���擾
	FString EditorPath = FPlatformProcess::ExecutablePath();

	// ���݊J���Ă���v���W�F�N�g�̃p�X���擾
	FString ProjectFilePath = FPaths::GetProjectFilePath();

	if (!FPaths::FileExists(EditorPath)) {
		UE_LOG(LogTemp, Error, TEXT("UnrealEditor.exe was not found at: %s"),
		       *EditorPath);
		return;
	}

	if (!FPaths::FileExists(ProjectFilePath)) {
		UE_LOG(LogTemp, Error, TEXT("uproject was not found at: %s"),
		       *ProjectFilePath);
		return;
	}

	// �R�}���h���C���������쐬
	FString CommandLineArgs =
	    FString::Printf(TEXT("\"%s\" %s -Port=%d -server -log"), *ProjectFilePath,
	                    *MapName, Port);

	// �v���Z�X���N��
	ProcessHandle =
	    FPlatformProcess::CreateProc(*EditorPath, *CommandLineArgs, true, false,
	                                 false, nullptr, 0, nullptr, nullptr);

	if (!ProcessHandle.IsValid()) {
		UE_LOG(LogTemp, Error, TEXT("UnrealEditor �v���Z�X�̋N���Ɏ��s���܂����B"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("UnrealEditor �v���Z�X���N�����܂���: %s %s"),
	       *EditorPath, *CommandLineArgs);
}

void ServerProcess::Kill() {
	if (ProcessHandle.IsValid()) {
		FPlatformProcess::TerminateProc(ProcessHandle);
		FPlatformProcess::CloseProc(ProcessHandle);
		ProcessHandle.Reset();
		UE_LOG(LogTemp, Log, TEXT("UnrealEditor �v���Z�X���I�����܂����B"));
	}
}

ServerProcess::~ServerProcess() {
	Kill();
}
