# BS4484 PROJECT V2.4

Projeto Android nativo importado do arquivo `BS4484 PROJECT V2.4.zip` fornecido pelo usuário.

## Parâmetros do projeto

- `applicationId`: `com.akn`
- `compileSdkVersion`: 30; `minSdkVersion`: 19; `targetSdkVersion`: 29
- ABIs configuradas no APK universal: `armeabi-v7a` e `arm64-v8a`
- Fontes nativas e build via NDK em `src/main/jni/`
- Bibliotecas nativas pré-compiladas e recursos Android em `src/main/`

## Build no GitHub Actions

O workflow `.github/workflows/android-apk.yml` está configurado para um runner `macos-14`. Ele instala Java 17, Android SDK 30, Build Tools 30.0.3, NDK LTS 30.0.16248370 e Gradle 7.6.4; compila as ABIs `armeabi-v7a` e `arm64-v8a` e, após uma compilação bem-sucedida, publica o APK debug como artefato da execução por 14 dias.

## Estado atual e ressalva

O ZIP recebido **não contém** os fontes `src/main/java/com/akn/MainActivity.java` e `src/main/java/com/akn/MenuService.java`, embora o manifesto faça referência a essas classes. Foi adicionado um setup Gradle e o manifesto foi corrigido. O workflow agora tenta compilar mesmo assim e registra um aviso; se a compilação técnica passar, o APK pode ser baixado como artefato da execução. Como as classes estão ausentes, não se pode garantir que o aplicativo abra ou funcione corretamente. Não foi incluído APK no commit.

## Observação sobre o conteúdo

Os fontes incluem referências a funções de modificação de jogo (por exemplo, aimbot, ESP, chams e hooks). Este repositório é público conforme solicitado; verifique direitos de redistribuição e regras do jogo antes de reutilizar o conteúdo.

Não foi fornecida licença no pacote. Na ausência de licença, não se presume permissão de reutilização por terceiros.
