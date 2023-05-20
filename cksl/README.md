# KSL

> License: MIT

一个玩具语言，自娱自乐

## 语法示例

### 常量

```ksl
114; (* Integer *)

5.14; (* Float *)

"Are you ok?"; (* String *)

#Pi (* Symbol *)
```

### 函数

```ksl
F[x: Integer, y: Float, z: String]: String :=
  StringJoin[{ToString[Round[AsFloat[x] - y]], z}];

Print[F[1, 0.4, " yes"]]; (* => "1 yes" *)
```

### 类型

```ksl
(* only list like type support, access by Get and Ident *)
TypeBind[SelfType, { x: Integer, y: Integer }];

p: SelfType = { x := 10, y := 20 };

Print[Get[p, x] + Get[p, y]]; (* => 30 *)
```
