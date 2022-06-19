apple = dependency('appleframeworks', modules: [
    'Foundation', 'Cocoa'
])

executable('prism',

           '../axabl/AXUIElement.m',
           '../axabl/NSWorkspace.m',

           'prism.m',
           
           install: true,
           objc_args: ['-F/System/Library/PrivateFrameworks'],
           link_args: ['-F/System/Library/PrivateFrameworks', '-framework', 'SkyLight'],
           dependencies: [apple]
)