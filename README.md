# BS4484 PROJECT V2.4

Projeto Android nativo importado do arquivo `BS4484 PROJECT V2.4.zip` fornecido pelo usuário.

## Parâmetros do projeto

- `applicationId`: `com.akn`
- `compileSdkVersion`: 30; `minSdkVersion`: 19; `targetSdkVersion`: 29
- ABI configurada: `armeabi-v7a`
- Fontes nativas e build via NDK em `src/main/jni/`
- Bibliotecas nativas pré-compiladas e recursos Android em `src/main/`

## Build no GitHub Actions

O workflow `.github/workflows/android-apk.yml` está configurado para um runner `macos-14`. Ele instala Java 17, Android SDK 30, Build Tools 30.0.3, NDK 21.4.7075529 e Gradle 7.6.4; após uma compilação bem-sucedida, publica o APK debug como artefato da execução por 14 dias.

## Bloqueio atual

O ZIP recebido **não contém** os fontes `src/main/java/com/akn/MainActivity.java` e `src/main/java/com/akn/MenuService.java`, embora o manifesto faça referência a essas classes. Também não contém wrapper/configuração Gradle funcional. Foi adicionado um setup Gradle e o manifesto foi corrigido, mas o APK **não pode ser compilado até que os dois fontes originais sejam restaurados**. O workflow para com erro explícito enquanto estiverem ausentes, em vez de criar um APK incompleto.

Nenhum APK está incluído neste repositório.

## Observação sobre o conteúdo

Os fontes incluem referências a funções de modificação de jogo (por exemplo, aimbot, ESP, chams e hooks). Este repositório é público conforme solicitado; verifique direitos de redistribuição e regras do jogo antes de reutilizar o conteúdo.

Não foi fornecida licença no pacote. Na ausência de licença, não se presume permissão de reutilização por terceiros.
