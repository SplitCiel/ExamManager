Sub ExamManager() 'Password is OverTheLimit
    
    Call Reset(False)
    
    Dim exam As Long, input1 As String, i As Long
    
    input1 = Range("F2").Value
        
    If CheckInput(input1, False) = False Then '入力が正しくないとき
        Call Error(1)
        Exit Sub
    End If
    
    exam = Val(input1) '入力が正しければExamにInput1を代入
    
    If exam < 1 Or exam > 10000 Then '1万を超える入力は不正とする
        Call Error(2)
        Exit Sub
    End If
    
    Dim score As Long, min As Long, max As Long, variance As Currency, sd As Currency, sum As Currency, avg As Currency, gap As Currency
    min = 10001 '範囲外の大きい数
    max = -10001 '範囲外の小さい数
    variance = 0
    sd = 0
    sum = 0
        
    For i = 1 To exam
    
        input1 = Cells(i + 1, 2).Value '得点を取得
        
        If CheckInput(input1, True) = False Then 'マイナスがついていてもよい
            Call Error(1)
            Exit Sub
        End If
            
        score = Val(input1)
        
        If Abs(score) > 10000 Then '±1万を超える入力は不正とする
            Call Error(2)
            Exit Sub
        End If
        
        sum = sum + score
        
        If score < min Then
            min = score
        End If
        
        If score > max Then
            max = score
        End If
        
    Next i
        
    avg = WorksheetFunction.Round(sum / exam, 1)
    
    For i = 1 To exam
        score = Cells(i + 1, 2).Value
        variance = variance + (CCur(score) - avg) * (CCur(score) - avg) 'ScoreとAvgの差の2乗をVarianceに加え続ける
    Next i
    
    variance = WorksheetFunction.Round(variance / exam, 0) 'Examで除算し分散を出す
    
    sd = WorksheetFunction.Round(Sqr(variance), 1)
    
    Call PrepareForOutput(exam) '出力するため、太枠などを用意する
    
    For i = 1 To exam
        
        Cells(i + 1, 1) = i 'IDを出力
        
        score = Cells(i + 1, 2).Value
        
        If sd = 0 Then 'ゼロ除算を防ぐ
            Cells(i + 1, 3).Value = 50
        Else
            Cells(i + 1, 3).Value = WorksheetFunction.Round(((10 * (CCur(score) - avg) / sd) + 50), 1)
        End If
        
        Cells(i + 1, 4).Value = WorksheetFunction.Rank_Eq(score, Range(Cells(2, 2), Cells(exam + 1, 2)))
    Next i
    
    Range("H2").Value = avg
    Range("I2").Value = min
    Range("J2").Value = max
    Range("H4").Value = sd
    Range("J4").Value = "成功"
    
    
    Range(Cells(2, 2), Cells(exam + 1, 2)).Locked = True '得点欄をロックすべきとする
    Range("F2").Locked = True '人数欄をロックすべきとする
    ActiveSheet.Protect ("OverTheLimit") 'ロックすべきセルをすべてロックする
    
End Sub

Sub AllReset()
    
    Call Reset(True)

End Sub

Function CheckInput(input1 As String, signed As Boolean) As Boolean
    
    Dim i As Integer
    
    If Len(input1) > 6 Or Len(input1) = 0 Then '6字を超える入力は不正とする
        CheckInput = False
        Exit Function
    End If
    
    For i = 1 To Len(input1)
        
        Dim c As Integer
        c = Asc(Mid(input1, i, 1)) 'CはInput1のI文字目のASCIICodeを返す
        
        If signed = True Then 'マイナスの入力に対応するとき
            
            If (c <> 43) And (c <> 45) And ((c < 48) Or (c > 57)) Then 'Cが符号でも数でもないとき
                CheckInput = False
                Exit Function
            ElseIf c = 43 Or c = 45 Then 'Cが符号のとき
            
                If i <> 1 Then '1文字目以外
                    CheckInput = False
                    Exit Function
                End If
            
            End If
            
        Else 'マイナスの入力に対応しないとき
            
            If (c <> 43) And ((c < 48) Or (c > 57)) Then 'Cが正符号または数でないとき
                CheckInput = False
                Exit Function
            ElseIf c = 43 Then 'Cが正符号のとき
            
                If i <> 1 Then '1文字目以外
                    CheckInput = False
                    Exit Function
                End If
                
            End If
                
        End If
        
    Next i
    
    CheckInput = True
End Function
    
Function Error(code As Integer)

    If code = 1 Then
        MsgBox "入力は1～6字かつ整数のみとしてください。"
    ElseIf code = 2 Then
        MsgBox "入力が大きすぎるか、小さすぎます。"
    Else
        MsgBox "正しく入力されませんでした。"
    End If
    
    Call Reset(False)
    ActiveSheet.Unprotect ("OverTheLimit") 'アンロックする
    Range("J4").Value = "失敗"
    ActiveSheet.Protect ("OverTheLimit") 'ロックすべきセルをすべてロックする
    
End Function
    
Function Reset(all As Boolean)
    
    EnableSelection = xlNoRestrictions 'セルを選べるようにする
    ActiveSheet.Unprotect ("OverTheLimit") 'アンロックする

    Dim i As Integer, j As Integer, bound As Long
    
    For i = 1 To 4
    
        If i <> 2 Then '得点はクリアしない
        
            bound = Cells(Rows.Count, i).End(xlUp).Row '最も下にある中身のあるセルの行数
            
            If bound <> 1 Then 'Boundが1ならクリアしない
                Range(Cells(2, i), Cells(bound, i)).ClearContents '下までクリアする
            End If
        
        End If
        
    Next i
    
    For i = 1 To 2 'クリアすべきは2と4であるため2倍する
        For j = 8 To 10
            If i <> 2 Or j <> 9 Then 'Iが2でJが9のときはクリアしない
                Cells(i * 2, j).ClearContents
            End If
        Next j
    Next i
    
    If all = True Then 'AllがTrueなら得点や人数もクリアする
    
        Range("F2").ClearContents
        
        bound = Cells(Rows.Count, 2).End(xlUp).Row 'Boundは中身のある最も下のセルの行数を表す
        
        If bound <> 1 Then '1行目しか入力されていないならクリアしない
            Range(Cells(2, 2), Cells(bound, 2)).ClearContents '得点を下までクリア
        End If
    
    End If
    
    Range("A1:D10001").Borders.LineStyle = xlContinuous 'セルを線で細分化する
    Range("A1:D10001").BorderAround LineStyle:=xlContinuous, Weight:=xlThick '外枠を囲う
    Range("A1:D10001").HorizontalAlignment = xlCenter '文字を中央にそろえる
    
    Range("F1:F2").Borders.LineStyle = xlContinuous 'セルを線で細分化する
    Range("F1:F2").BorderAround LineStyle:=xlContinuous, Weight:=xlThick '外枠を囲う
    Range("F1:F2").HorizontalAlignment = xlCenter '文字を中央にそろえる
    
    Range("H1:J4").Borders.LineStyle = xlContinuous 'セルを線で細分化する
    Range("H1:J4").BorderAround LineStyle:=xlContinuous, Weight:=xlThick '外枠を囲う
    Range("H1:J4").HorizontalAlignment = xlCenter '文字を中央にそろえる
    
    Range("I3:I4").Borders(xlDiagonalUp).LineStyle = True 'セルを「／」にする
    
    Range("F2").Interior.Color = RGB(169, 208, 142) 'セルを緑色に
    Range("B2:B10001").Interior.Color = RGB(169, 208, 142)
    
    Range("A1") = "ID"
    Range("B1") = "得点"
    Range("C1") = "偏差値"
    Range("D1") = "順位"
    
    Range("F1") = "受験者数"
    
    Range("H1") = "平均"
    Range("I1") = "最小"
    Range("J1") = "最大"
    Range("H3") = "標準偏差"
    Range("J3") = "結果"
    
    'セルの大きさをそろえる
    Range("A:O").ColumnWidth = 8.1
    Range("1:10001").RowHeight = 18
    
    '文字の色と大きさをそろえる
    Range("A1:O10001").Font.Color = RGB(0, 0, 0)
    Range("A1:O10001").Font.Size = 11
    
    Range("F6").HorizontalAlignment = xlLeft '文字を左にそろえる
    Range("F6") = "F2セルには1万までの自然数を、B2以下のセルには±1万までの整数を上から入力してください。"
    
    '文字の色を緑に
    Range("F6").Characters(Start:=1, Length:=4).Font.Color = RGB(112, 173, 71)
    Range("F6").Characters(Start:=17, Length:=7).Font.Color = RGB(112, 173, 71)
    
    '小数第1位まで表示
    Range("C2:C10001").NumberFormat = "0.0"
    Range("H2").NumberFormat = "0.0"
    Range("H4").NumberFormat = "0.0"
    
    Range("A1:O10001").Locked = True 'すべてロックすべきに
    Range("B2:B10001").Locked = False '得点欄をアンロックすべきに
    Range("F2").Locked = False '人数欄をアンロックすべきに
    
    ActiveSheet.Protect ("OverTheLimit") 'ロックすべきセルをすべてロックする
    
End Function

Function PrepareForOutput(exam As Long)

    ActiveSheet.Unprotect ("OverTheLimit") 'アンロックする
    
    If exam <> 10000 Then 'Examが10000の場合はクリアすべきでない
        Range(Cells(exam + 2, 1), Cells(10001, 4)).Clear '対象より下のセルをクリア
    End If
    
    Range("F6").Clear
    Range(Cells(exam + 2, 1), Cells(exam + 2, 4)).Borders(xlEdgeTop).LineStyle = xlContinuous '下部を線で区切る
    Range(Cells(exam + 2, 1), Cells(exam + 2, 4)).Borders(xlEdgeTop).Weight = xlThick '下部を太線に
    
    Range(Cells(2, 2), Cells(exam + 1, 2)).Interior.ColorIndex = xlNone 'セルの塗りをオフに
    Range("F2").Interior.ColorIndex = xlNone
    
End Function