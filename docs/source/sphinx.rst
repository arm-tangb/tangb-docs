
==============================
Sphinx
==============================

+--------+------------+-----------------------+-----------------------------------------------+
| 版本   | 更新日期   | 编辑                  | 说明                                          |
+========+============+=======================+===============================================+
| v1.0.1 | 2022.09.06 | 唐斌                  | 整理参考资料                                  |
+--------+------------+-----------------------+-----------------------------------------------+
| v1.0.0 | 2022.08.26 | 唐斌: 7120094@qq.com  | 初始版本                                      |
+--------+------------+-----------------------+-----------------------------------------------+


参考资料
==============================

Using Sphinx: 
`[英文] <https://www.sphinx-doc.org/en/master/usage/index.html>`_
`[中文] <https://www.sphinx-doc.org/zh_CN/master/usage/index.html>`_

reStructuredText `[docutils] <https://docutils.sourceforge.io/rst.html>`_

reStructuredText 参考: 
`[sphinx] <https://www.sphinx-doc.org/en/master/usage/restructuredtext/index.html>`_
`[快速开始] <https://docutils.sourceforge.io/docs/user/rst/quickref.html>`_
`[标记规范] <https://docutils.sourceforge.io/docs/ref/rst/restructuredtext.html>`_
`[指令] <https://docutils.sourceforge.io/docs/ref/rst/directives.html>`_

Using Sphinx
==============================

Getting Started
------------------------------

配置文档源
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: shell

    sphinx-quickstart

    > 项目语种 [en]: zh_CN


定义文档结构
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.. code-block::

    .. toctree::
       :maxdepth: 1
       :caption: 工具

       doc/sphinx

添加内容
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

略

运行创建工具
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

+ 手动构建

.. code-block:: shell

    sphinx-build -b html . _build/html
    # or
    make.bat  html  # Windows
    make html       # Linux

+ 自动构建并本地预览

.. code-block:: shell

    sphinx-autobuild . _build/html

`本地访问`_

.. _本地访问: http://127.0.0.1:8000/

+ 远程访问

进入到生成的_build/html目录，运行如下指令：

.. code-block:: shell

    python3 -m http.server --bind 0.0.0.0 8000

`远程访问`_

.. _远程访问: http://docs.tangb.site:63980
