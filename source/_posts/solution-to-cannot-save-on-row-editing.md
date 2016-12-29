title: extjs 解决rowEditing不满足allowBlank时，无法save的问题
tags: [extjs,web前端]
categories: 设计开发
date: 2014-10-22 03:58:26
---

<span style="white-space:pre"></span>最近一个项目使用了extjs，在用rowEditing的时候遇到了棘手的问题，当某一column的editor使用了属性allowBlank:true时，启用编辑时会报错（“该行不能为空”），这时save按钮是灰色的无法提交。然而填了该行，save仍然无法提交。

<span style="white-space:pre"></span>经过google之后发现这是个bug，最好的解决方法就是重写rowEditing~ORZ

<!--more-->

JS如下：

```js
Ext.override(Ext.grid.RowEditor, {

    addFieldsForColumn: function(column, initial) {

        var me = this,

        i, length, field;

        if (Ext.isArray(column)) {

            for (i = 0, length = column.length; i < length; i++) {

                me.addFieldsForColumn(column[i], initial);

            }

            return;

        }

        if (column.getEditor) {

            field = column.getEditor(null, {

                xtype: &#39;displayfield&#39;,

                getModelData: function() {

                    return null;

                }

            });

            if (column.align === &#39;right&#39;) {

                field.fieldStyle = &#39;text-align:right&#39;;

            }

            if (column.xtype === &#39;actioncolumn&#39;) {

                field.fieldCls += &#39; &#39; + Ext.baseCSSPrefix + &#39;form-action-col-field&#39;;

            }

            if (me.isVisible() && me.context) {

                if (field.is(&#39;displayfield&#39;)) {

                    me.renderColumnData(field, me.context.record, column);

                } else {

                    field.suspendEvents();

                    field.setValue(me.context.record.get(column.dataIndex));

                    field.resumeEvents();

                }

            }

            if (column.hidden) {

                me.onColumnHide(column);

            } else if (column.rendered && !initial) {

                me.onColumnShow(column);

            }

            // -- start edit

            me.mon(field, &#39;change&#39;, me.onFieldChange, me);

            // -- end edit

        }

    }

});

//重写RowEditor
```

如果大家有遇到该问题的，只要在js文件里添加该段代码即可～