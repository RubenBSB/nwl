from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="nwl",  # as it would be imported
                               # may include packages/namespaces separated by `.`

            sources=["nwlmodule.c", "needle.c"], # all sources are compiled into a single binary file
        ),
    ]
)