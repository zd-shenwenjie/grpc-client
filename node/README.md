# Management

using typescripy + grpc

#### Getting started
- Clone the repository
```
git@github.com:zd-shenwenjie/management.git
```
- Init submodule (proto)
```
git submodule init
```
- Update submodule
```
git submodule update
```
This repository contains a protoc plugin that generates TypeScript declarations (.d.ts files) that match the JavaScript output of protoc --js_out=import_style=commonjs,binary. This plugin can also output service definitions as both .js and .d.ts files in the structure required by grpc-web, and as .d.ts files in the structure required by grpc-node.
```
npm run build:proto
```

#### How to run
```
npm install
npm run build
npm start
```